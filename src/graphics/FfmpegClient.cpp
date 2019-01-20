//
// Created by bfrasure on 1/19/19.
//

#include "FfmpegClient.h"

void FfmpegClient::createVideo(const std::time_t & start) {
    std::ostringstream stream;
    stream << std::put_time(std::localtime(&start), "%F %T");
    string ffmpegCommandAndOptions = "ffmpeg  -i ./output/outFrame%05d.jpg -framerate 1 -c:v libx264 -crf 18 -pix_fmt yuv420p \"";
    string outDirectory = "./WorthyVideos/";
    string outVideoFormat = ".mp4\"";
    string fullCommand = ffmpegCommandAndOptions + outDirectory + stream.str() + outVideoFormat;
    redi::ipstream in(fullCommand);
    std::string ffmpegOutput;
    while (in >> ffmpegOutput) { /* Do nothing */ }
    redi::ipstream cleanup("rm -f ./output/*");

}
