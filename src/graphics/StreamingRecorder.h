#ifndef STREAMING_RECORDER_H_
#define STREAMING_RECORDER_H_

#include <GL/glut.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <atomic>

using namespace std;

/*! \brief Records frames to temp directory, encodes to video on finalize
 *
 *  Writes raw frames to /tmp (RAM-backed on macOS) during simulation,
 *  then encodes all frames to H.264 video via ffmpeg at the end.
 */
class StreamingRecorder {
public:
    StreamingRecorder(unsigned int width, unsigned int height, const string& outputPath);
    ~StreamingRecorder();

    /*! \brief Captures current frame and writes to temp directory */
    bool captureFrame();

    /*! \brief Encodes all captured frames to video, keeps recording active */
    bool createVideoSoFar();

    /*! \brief Encodes all frames to video and cleans up temp files */
    void finalize();

private:
    unsigned int width;
    unsigned int height;
    size_t frameSize;
    string outputPath;
    string tempDir;
    GLubyte* captureBuffer;
    atomic<bool> isRecording;
    atomic<int> frameCount;
};

#endif
