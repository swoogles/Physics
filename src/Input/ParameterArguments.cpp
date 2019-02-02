//
// Created by bfrasure on 2/1/19.
//

#include "ParameterArguments.h"

bool ParameterArguments::parseRecordingParameters(char **argv) {
    char recording = argv[1][0];
    if ( recording == 'r') {
        return true;
    } else if (recording == 'x') {
        return false;
    } else {
        cout << "Bad recording value! Must be 'x' or 'r'" << endl;
        exit(1);
    }
}
