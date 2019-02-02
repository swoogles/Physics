//
// Created by bfrasure on 2/1/19.
//

#include "ParameterArguments.h"

bool ParameterArguments::parseRecordingParameters(char **argv) {
}

CraftedSimulation parseSimulationChoise(char sim) {
    if ( sim == '0' ) {
    } else if ( sim == '1' ) {
        return CraftedSimulation::BODY_FORMATION;
    } else if ( sim == '2' ) {
        return CraftedSimulation::DISRUPT_GROUP;
    } else if ( sim == '3' ) {
        return CraftedSimulation::QUADRANT_TESTING;
    } else if ( sim == '4') {
        return CraftedSimulation::MULTIPLE_ORBITERS;
    } else {
        cout << "Simulation choice does not match any existing choices: " <<  sim << endl;
        exit(1);
    }


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
    craftedSimulation = parseSimulationChoise(argv[2][0]);
}


bool ParameterArguments::isRecording() const {
    return recording;
}

CraftedSimulation ParameterArguments::getCraftedSimulation() const {
    return craftedSimulation;
}
