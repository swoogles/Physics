#ifndef PHYSICS_RUNOPTIONS_H
#define PHYSICS_RUNOPTIONS_H

#include <string>

using std::string;

/*! \brief Everything a single run needs to know that isn't physics.
 *
 *  Parsed from the command line, with defaults that reproduce the old
 *  behaviour so `./build/PhysicsSandbox r 1` keeps working.
 */
class RunOptions {
public:
    RunOptions(int argc, char **argv);

    string configPath = "simulation.properties";

    //! Empty means "derive from the start timestamp", as it always has.
    string outputPath;

    //! 0 means "pick one from the clock and record it in the sidecar".
    unsigned int seed = 0;

    bool recording = false;

    int width = 1920;
    int height = 1080;

    //! Stop after this many recorded frames. 0 disables the limit.
    int maxFrames = 0;

    //! Wall-clock cap in seconds. 0 means "use maximum_runtime from the config".
    long maxSeconds = 0;

    //! Build the scenario, print the setup diagnostics as JSON, and exit.
    bool printSetupOnly = false;

    //! Write a .json sidecar next to the video describing the run.
    bool writeSidecar = true;

    //! Resolved once so both the video and its sidecar agree.
    string sidecarPath() const;

    static void printUsage();

private:
    void applyResolutionPreset(const string &preset);
};

#endif //PHYSICS_RUNOPTIONS_H
