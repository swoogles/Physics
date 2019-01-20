//
// Created by bfrasure on 1/19/19.
//

#ifndef PHYSICS_FFMEPCLIENT_H
#define PHYSICS_FFMEPCLIENT_H

#include <chrono>
#include <iomanip>
#include <string>

#include <lib/pstream.h>

using namespace std;

// Look to use generic VideoClient name here, and make FffmpegClient an implementation detail.
class FfmpegClient {
public:
    void createVideo(const std::time_t &  start);
};


#endif //PHYSICS_FFMEPCLIENT_H
