//
// Created by bfrasure on 2/1/19.
//

#ifndef PHYSICS_PARAMETERARGUMENTS_H
#define PHYSICS_PARAMETERARGUMENTS_H

#include <iostream>
#include <Physics/CraftedSimulation.h>

using namespace std;

class ParameterArguments {
public:
    ParameterArguments(char **argv);

    bool parseRecordingParameters(char **argv);
private:
    char **argv;
    bool recording;
    CraftedSimulation craftedSimulation;
public:
    CraftedSimulation getCraftedSimulation() const;

public:
    bool isRecording() const;

};


#endif //PHYSICS_PARAMETERARGUMENTS_H
