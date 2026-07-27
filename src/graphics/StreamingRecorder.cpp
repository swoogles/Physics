#include "StreamingRecorder.h"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <fcntl.h>
#include <algorithm>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>

/*! h264 with yuv420p subsamples chroma 2x2, so it cannot encode an odd width or
 *  height - ffmpeg exits without writing a single packet and leaves a 0-byte
 *  file behind. Window sizes are whatever the window manager hands back, so an
 *  odd one is a coin flip. Losing a row or column of pixels is invisible;
 *  losing the whole run to a failed encode is not.
 */
static unsigned int roundDownToEven(unsigned int value) {
    return value - (value % 2);
}

StreamingRecorder::StreamingRecorder(unsigned int requestedWidth, unsigned int requestedHeight, const string& outputPath)
    : width(roundDownToEven(requestedWidth))
    , height(roundDownToEven(requestedHeight))
    , frameSize(roundDownToEven(requestedWidth) * roundDownToEven(requestedHeight) * 3)
    , outputPath(outputPath)
    , captureBuffer(nullptr)
    , isRecording(false)
    , frameCount(0)
{
    // Allocate capture buffer
    captureBuffer = new GLubyte[frameSize];

    // Create temp directory for raw frames
    tempDir = "/tmp/physics_frames_" + to_string(time(nullptr));

    int mkdirResult = mkdir(tempDir.c_str(), 0755);
    if (mkdirResult != 0 && errno != EEXIST) {
        cerr << "ERROR: Failed to create temp directory: " << tempDir
             << " errno=" << errno << " (" << strerror(errno) << ")" << endl;
    }

    // Verify directory was created
    DIR* dir = opendir(tempDir.c_str());
    if (dir) {
        closedir(dir);
        cout << "Temp directory created successfully: " << tempDir << endl;
    } else {
        cerr << "ERROR: Temp directory does not exist after creation!" << endl;
    }

    // Create output directory
    mkdir("./WorthyVideos", 0755);

    isRecording = true;
    if (width != requestedWidth || height != requestedHeight) {
        cout << "Capture size trimmed to even dimensions: "
             << requestedWidth << "x" << requestedHeight
             << " -> " << width << "x" << height << " (h264 requires even)" << endl;
    }
    cout << "Recording initialized. Frame size: " << width << "x" << height
         << " (" << frameSize << " bytes per frame)" << endl;
    cout << "Final output will be: " << outputPath << endl;
    cout.flush();
}

StreamingRecorder::~StreamingRecorder() {
    finalize();

    if (captureBuffer) {
        delete[] captureBuffer;
        captureBuffer = nullptr;
    }
}

bool StreamingRecorder::captureFrame() {
    if (!isRecording) {
        return false;
    }

    int currentFrame = frameCount.load();

    // Read pixel data from OpenGL front buffer (after swap)
    glReadBuffer(GL_FRONT);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, captureBuffer);
    glReadBuffer(GL_BACK);  // Restore default

    // Create filename with frame number
    ostringstream filename;
    filename << tempDir << "/frame_" << setw(6) << setfill('0') << currentFrame << ".raw";

    // Write raw frame to temp file
    FILE* f = fopen(filename.str().c_str(), "wb");
    if (f) {
        // Flip vertically while writing (OpenGL is bottom-up, video is top-down)
        const int numComponents = 3;
        for (int y = height - 1; y >= 0; y--) {
            fwrite(captureBuffer + y * width * numComponents, 1, width * numComponents, f);
        }
        fclose(f);

        frameCount++;

        if (frameCount % 100 == 0) {
            cout << "Captured " << frameCount << " frames" << endl;
            cout.flush();
        }

        return true;
    } else {
        cerr << "Failed to write frame: " << filename.str()
             << " errno=" << errno << " (" << strerror(errno) << ")" << endl;
        cerr.flush();
        return false;
    }
}

bool StreamingRecorder::createVideoSoFar() {
    int totalFrames = frameCount.load();
    cout << "\n=== CREATING VIDEO ===" << endl;
    cout << "Frame count: " << totalFrames << endl;
    cout << "Temp directory: " << tempDir << endl;
    cout.flush();

    if (totalFrames == 0) {
        cout << "No frames captured - nothing to encode" << endl;
        return false;
    }

    // Verify temp directory and count files
    DIR* dir = opendir(tempDir.c_str());
    if (!dir) {
        cerr << "ERROR: Cannot open temp directory: " << tempDir << endl;
        cerr << "errno=" << errno << " (" << strerror(errno) << ")" << endl;
        return false;
    }

    int fileCount = 0;
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (strstr(entry->d_name, ".raw") != nullptr) {
            fileCount++;
            if (fileCount <= 3) {
                cout << "  Found: " << entry->d_name << endl;
            }
        }
    }
    closedir(dir);

    cout << "Total .raw files found: " << fileCount << endl;
    cout.flush();

    if (fileCount == 0) {
        cerr << "ERROR: No .raw files found in temp directory!" << endl;
        return false;
    }

    cout << "Encoding " << fileCount << " frames to video..." << endl;
    cout.flush();

    // Concatenate all raw frames into a single file for ffmpeg
    // (rawvideo demuxer expects a single continuous stream, not a sequence)
    string combinedFile = tempDir + "/combined.raw";
    cout << "Concatenating frames into single raw file..." << endl;
    cout.flush();

    ostringstream catCmd;
    catCmd << "cat";
    for (int i = 0; i < totalFrames; i++) {
        catCmd << " \"" << tempDir << "/frame_"
               << setw(6) << setfill('0') << i << ".raw\"";
    }
    catCmd << " > \"" << combinedFile << "\"";

    int catResult = system(catCmd.str().c_str());
    if (catResult != 0) {
        cerr << "ERROR: Failed to concatenate frames" << endl;
    }

    // Verify combined file
    struct stat st;
    if (stat(combinedFile.c_str(), &st) == 0) {
        cout << "Combined file: " << st.st_size << " bytes "
             << "(expected " << (frameSize * totalFrames) << ")" << endl;
    } else {
        cerr << "ERROR: Combined file not created!" << endl;
        return false;
    }
    cout.flush();

    // Build ffmpeg command
    ostringstream ffmpegCmd;
    ffmpegCmd << "ffmpeg -y "
              << "-f rawvideo "
              << "-pixel_format rgb24 "
              << "-video_size " << width << "x" << height << " "
              << "-framerate 24 "
              << "-i \"" << combinedFile << "\" "
              << "-c:v libx264 "
              << "-preset fast "
              << "-crf 18 "
              << "-pix_fmt yuv420p "
              << "-r 30 "
              << "\"" << outputPath << "\" "
              << "2>&1";

    cout << "Running: " << ffmpegCmd.str() << endl;
    cout.flush();

    // Run ffmpeg
    FILE* pipe = popen(ffmpegCmd.str().c_str(), "r");
    if (pipe) {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            cout << buffer;
        }
        int result = pclose(pipe);
        if (result == 0) {
            cout << "Video created successfully: " << outputPath << endl;

            // Copy to latest.mp4 for easy access
            string latestPath = "./WorthyVideos/latest.mp4";
            string copyCmd = "cp \"" + outputPath + "\" \"" + latestPath + "\"";
            if (system(copyCmd.c_str()) == 0) {
                cout << "Copied to: " << latestPath << endl;
            }

            // Remove the combined.raw file to save space, but keep individual frames
            string rmCombined = "rm -f \"" + combinedFile + "\"";
            system(rmCombined.c_str());
            return true;
        } else {
            cerr << "ffmpeg returned error code: " << result << endl;
            return false;
        }
    }
    return false;
}

void StreamingRecorder::finalize() {
    if (!isRecording) {
        return;
    }

    isRecording = false;

    cout << "\n=== FINALIZING VIDEO ===" << endl;
    createVideoSoFar();

    // Cleanup temp files
    cout << "Cleaning up temp files..." << endl;
    string cleanupCmd = "rm -rf \"" + tempDir + "\"";
    system(cleanupCmd.c_str());

    cout << "Done!" << endl;
}
