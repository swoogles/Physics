//
// Created by bfrasure on 1/19/19.
//

#include "FfmpegClient.h"

#include <iomanip>
#include <string>
#include <lib/pstream.h>

void FfmpegClient::createVideo(const std::time_t & start) {
    std::ostringstream stream;
    stream << std::put_time(std::localtime(&start), "%F %T");
//    string ffmpegCommandAndOptions = "ffmpeg  -i ./output/outFrame%05d.jpg -framerate 1 -c:v libx264 -crf 18 -pix_fmt yuv420p \"";
    // Consider 2160p in future
    string ffmpegCommandAndOptions = "ffmpeg  -i ./output/outFrame%05d.jpg -framerate 1 -c:v libx264 -crf 18 -pix_fmt yuv420p -vf scale=-2:1440:flags=neighbor -r 30 -y \"";
    string outDirectory = "./WorthyVideos/";
    string outVideoFormat = ".mp4\"";
    string fullCommand = ffmpegCommandAndOptions + outDirectory + stream.str() + outVideoFormat;
    redi::ipstream in(fullCommand);
    std::string ffmpegOutput;
    while (in >> ffmpegOutput) { /* Do nothing */ }

}

void FfmpegClient::cleanupFrames() {
    redi::ipstream cleanup("rm -f ./output/*");

}
