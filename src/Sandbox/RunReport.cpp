#include "RunReport.h"

#include <cstdio>
#include <fstream>
#include <iomanip>
#include <sstream>

using std::string;

namespace {

    string escape(const string &value) {
        string escaped;
        for (char character : value) {
            if (character == '"' || character == '\\') {
                escaped += '\\';
            }
            escaped += character;
        }
        return escaped;
    }

    string vectorJson(const PhysicalVector &vector) {
        std::ostringstream json;
        json << "[" << vector.x() << ", " << vector.y() << ", " << vector.z() << "]";
        return json.str();
    }

    string groupJson(const GroupSpec &group) {
        std::ostringstream json;
        json << "{"
             << "\"label\": \"" << escape(group.label) << "\""
             << ", \"count\": " << group.count
             << ", \"mass_kg\": " << group.mass
             << ", \"radius_m\": " << group.radius
             << ", \"dispersion\": " << group.dispersion
             << ", \"virial_ratio\": " << group.virialRatio
             << ", \"position\": " << vectorJson(group.position)
             << ", \"velocity\": " << vectorJson(group.velocity)
             << ", \"spin\": " << vectorJson(group.spin)
             << ", \"color\": " << vectorJson(group.color)
             << "}";
        return json.str();
    }

}

string RunReport::currentGitRevision() {
    FILE *pipe = popen("git rev-parse --short HEAD 2>/dev/null", "r");
    if (!pipe) {
        return "unknown";
    }

    char buffer[64] = {0};
    string revision;
    if (fgets(buffer, sizeof(buffer), pipe)) {
        revision = BillProperties::trim(string(buffer));
    }
    pclose(pipe);

    return revision.empty() ? "unknown" : revision;
}

RunReport::RunReport(const RunOptions &options,
                     const BillProperties &config,
                     const ScenarioSpec &scenario,
                     const SetupDiagnostics &diagnostics)
        : options(options),
          config(config),
          scenario(scenario),
          diagnostics(diagnostics),
          gitRevision(currentGitRevision()) {}

void RunReport::sample(int frame, const SimulationStats &stats) {
    history.push_back({frame,
                       stats.particleCount,
                       stats.mergesLastStep,
                       stats.largestMassFraction,
                       stats.rmsRadius});
}

void RunReport::finish(int framesRendered, double wallClockSeconds, const SimulationStats &finalStats) {
    this->framesRendered = framesRendered;
    this->wallClockSeconds = wallClockSeconds;
    this->finalStats = finalStats;
    this->finished = true;
}

string RunReport::toJson() const {
    std::ostringstream json;
    json << std::setprecision(6);

    json << "{\n";
    json << "  \"seed\": " << options.seed << ",\n";
    json << "  \"git_revision\": \"" << escape(gitRevision) << "\",\n";
    json << "  \"config_path\": \"" << escape(options.configPath) << "\",\n";
    json << "  \"video_path\": \"" << escape(options.outputPath) << "\",\n";
    json << "  \"recorded\": " << (options.recording ? "true" : "false") << ",\n";
    json << "  \"resolution\": [" << options.width << ", " << options.height << "],\n";
    json << "  \"frames_rendered\": " << framesRendered << ",\n";
    json << "  \"wall_clock_seconds\": " << wallClockSeconds << ",\n";
    json << "  \"completed\": " << (finished ? "true" : "false") << ",\n";

    json << "  \"scenario\": {\n";
    json << "    \"type\": \"" << escape(scenario.type) << "\",\n";
    json << "    \"name\": \"" << escape(scenario.name) << "\",\n";
    json << "    \"virial_ratio_target\": " << scenario.virialRatio << ",\n";
    json << "    \"knobs\": {";
    bool firstKnob = true;
    for (const auto &knob : scenario.knobs) {
        if (!firstKnob) {
            json << ", ";
        }
        json << "\"" << escape(knob.first) << "\": \"" << escape(knob.second) << "\"";
        firstKnob = false;
    }
    json << "},\n";
    json << "    \"groups\": [";
    for (size_t i = 0; i < scenario.groups.size(); i++) {
        json << (i == 0 ? "\n      " : ",\n      ") << groupJson(scenario.groups[i]);
    }
    json << "\n    ]\n";
    json << "  },\n";

    json << "  \"setup\": " << diagnostics.toJson() << ",\n";

    json << "  \"final\": {"
         << "\"particle_count\": " << finalStats.particleCount
         << ", \"total_merges\": " << finalStats.totalMerges
         << ", \"largest_mass_fraction\": " << finalStats.largestMassFraction
         << ", \"rms_radius_m\": " << finalStats.rmsRadius
         << ", \"largest_merge_jump_m\": " << finalStats.largestMergeJump
         << ", \"largest_visible_merge_jump_m\": " << finalStats.largestVisibleMergeJump
         << ", \"total_mass_kg\": " << finalStats.totalMass
         << "},\n";

    json << "  \"history\": [";
    for (size_t i = 0; i < history.size(); i++) {
        const StatSample &sample = history[i];
        json << (i == 0 ? "\n    " : ",\n    ")
             << "{\"frame\": " << sample.frame
             << ", \"particles\": " << sample.particleCount
             << ", \"merges\": " << sample.merges
             << ", \"largest_mass_fraction\": " << sample.largestMassFraction
             << ", \"rms_radius_m\": " << sample.rmsRadius
             << "}";
    }
    json << "\n  ],\n";

    json << "  \"config\": {";
    bool firstEntry = true;
    for (const auto &entry : config.all()) {
        if (!firstEntry) {
            json << ", ";
        }
        json << "\"" << escape(entry.first) << "\": \"" << escape(entry.second) << "\"";
        firstEntry = false;
    }
    json << "}\n";

    json << "}\n";

    return json.str();
}

bool RunReport::writeTo(const string &path) const {
    std::ofstream file(path);
    if (!file) {
        return false;
    }
    file << toJson();
    return file.good();
}
