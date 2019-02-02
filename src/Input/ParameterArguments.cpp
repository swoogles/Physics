//
// Created by bfrasure on 2/1/19.
//

#include "ParameterArguments.h"

bool ParameterArguments::parseRecordingParameters(char **argv) {
}

ParameterArguments::ParameterArguments(char **argv) : argv(argv) {
    char recording = argv[1][0];
    if ( recording == 'r') {
        this->recording =  true;
    } else if (recording == 'x') {
        this->recording =  false;
    } else {
        cout << "Bad recording value! Must be 'x' or 'r'" << endl;
        exit(1);
    }
}

bool ParameterArguments::isRecording() const {
    return recording;
}
