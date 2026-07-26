#include "RunOptions.h"

#include <cstdlib>
#include <iostream>

using std::cout;
using std::endl;

void RunOptions::printUsage() {
    cout << "Usage: PhysicsSandbox [options]\n"
         << "\n"
         << "  --config <path>     Properties file to run (default: simulation.properties)\n"
         << "  --out <path>        Output .mp4 path (default: ./WorthyVideos/<timestamp>.mp4)\n"
         << "  --seed <n>          RNG seed. 0 or omitted picks one and records it\n"
         << "  --record            Record video (same as the legacy 'r' argument)\n"
         << "  --no-record         Run without recording (legacy 'x')\n"
         << "  --res <preset>      720p | 1080p | 1440p | 4k (default: 1080p)\n"
         << "  --width <px>        Explicit frame width\n"
         << "  --height <px>       Explicit frame height\n"
         << "  --max-frames <n>    Stop after n frames. Gives every run the same length\n"
         << "  --max-seconds <n>   Wall-clock cap, overriding maximum_runtime\n"
         << "  --print-setup       Print scenario diagnostics as JSON and exit\n"
         << "  --no-sidecar        Skip the .json run report\n"
         << "  -h, --help          This message\n"
         << "\n"
         << "Legacy form still works: PhysicsSandbox r|x <simulation-number>\n"
         << endl;
}

void RunOptions::applyResolutionPreset(const string &preset) {
    if (preset == "720p") {
        width = 1280; height = 720;
    } else if (preset == "1080p") {
        width = 1920; height = 1080;
    } else if (preset == "1440p") {
        width = 2560; height = 1440;
    } else if (preset == "4k") {
        width = 3840; height = 2160;
    } else {
        cout << "Unknown resolution preset: " << preset << endl;
        printUsage();
        exit(1);
    }
}

RunOptions::RunOptions(int argc, char **argv) {
    auto requireValue = [argc, argv](int i, const char *flag) -> string {
        if (i + 1 >= argc) {
            cout << "Missing value for " << flag << endl;
            printUsage();
            exit(1);
        }
        return string(argv[i + 1]);
    };

    for (int i = 1; i < argc; i++) {
        string arg(argv[i]);

        if (arg == "--config") {
            configPath = requireValue(i, "--config"); i++;
        } else if (arg == "--out") {
            outputPath = requireValue(i, "--out"); i++;
        } else if (arg == "--seed") {
            seed = static_cast<unsigned int>(strtoul(requireValue(i, "--seed").c_str(), nullptr, 10)); i++;
        } else if (arg == "--record" || arg == "-r") {
            recording = true;
        } else if (arg == "--no-record") {
            recording = false;
        } else if (arg == "--res") {
            applyResolutionPreset(requireValue(i, "--res")); i++;
        } else if (arg == "--width") {
            width = atoi(requireValue(i, "--width").c_str()); i++;
        } else if (arg == "--height") {
            height = atoi(requireValue(i, "--height").c_str()); i++;
        } else if (arg == "--max-frames") {
            maxFrames = atoi(requireValue(i, "--max-frames").c_str()); i++;
        } else if (arg == "--max-seconds") {
            maxSeconds = strtol(requireValue(i, "--max-seconds").c_str(), nullptr, 10); i++;
        } else if (arg == "--print-setup") {
            printSetupOnly = true;
        } else if (arg == "--no-sidecar") {
            writeSidecar = false;
        } else if (arg == "-h" || arg == "--help") {
            printUsage();
            exit(0);
        } else if (arg == "r") {
            recording = true;      // legacy positional form
        } else if (arg == "x") {
            recording = false;     // legacy positional form
        } else if (arg.size() == 1 && arg[0] >= '0' && arg[0] <= '9') {
            // Legacy simulation-number argument. The scenario now comes from the
            // config file, so there is nothing left to select here.
        } else {
            cout << "Unrecognized argument: " << arg << endl;
            printUsage();
            exit(1);
        }
    }
}

string RunOptions::sidecarPath() const {
    if (outputPath.empty()) {
        return "";
    }

    const auto dot = outputPath.find_last_of('.');
    const auto slash = outputPath.find_last_of('/');
    if (dot == string::npos || (slash != string::npos && dot < slash)) {
        return outputPath + ".json";
    }
    return outputPath.substr(0, dot) + ".json";
}
