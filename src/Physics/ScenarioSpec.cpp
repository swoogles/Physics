#include "ScenarioSpec.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

using std::string;
using std::vector;

int ScenarioSpec::totalParticles() const {
    int total = 0;
    for (const auto &group : groups) {
        total += group.count;
    }
    return total;
}

double ScenarioSpec::totalMass() const {
    double total = 0;
    for (const auto &group : groups) {
        total += group.mass;
    }
    return total;
}

double ScenarioParser::parseDouble(const string &value, double fallback) {
    if (value.empty()) {
        return fallback;
    }
    char *end = nullptr;
    const double parsed = strtod(value.c_str(), &end);
    return end == value.c_str() ? fallback : parsed;
}

/*! Accepts "1,2,3" or a bare scalar, which becomes (0, 0, scalar) so that
 *  `spin=0.4` means "0.4 of break-up spin about z", the common case.
 */
PhysicalVector ScenarioParser::parseVector(const string &value, PhysicalVector fallback) {
    const string trimmed = BillProperties::trim(value);
    if (trimmed.empty()) {
        return fallback;
    }

    vector<double> parts;
    std::stringstream stream(trimmed);
    string piece;
    while (getline(stream, piece, ',')) {
        parts.push_back(parseDouble(BillProperties::trim(piece), 0.0));
    }

    if (parts.size() == 1) {
        return PhysicalVector(0, 0, (float)parts[0], false);
    }
    if (parts.size() >= 3) {
        return PhysicalVector((float)parts[0], (float)parts[1], (float)parts[2], false);
    }
    return fallback;
}

namespace {

    //! Palette used when a group doesn't name its own colour.
    PhysicalVector paletteColor(int index) {
        static const PhysicalVector palette[] = {
                PhysicalVector(1.0, 1.0, 1.0),
                PhysicalVector(0.0, 1.0, 1.0),
                PhysicalVector(1.0, 0.5, 0.0),
                PhysicalVector(0.6, 0.7, 1.0),
                PhysicalVector(1.0, 0.4, 0.7),
                PhysicalVector(0.5, 1.0, 0.5),
        };
        const int paletteSize = sizeof(palette) / sizeof(palette[0]);
        return palette[index % paletteSize];
    }

    //! Reads a key, remembering that it was consulted so the report can echo it.
    double knob(const BillProperties &properties, ScenarioSpec &spec, const string &key, double fallback) {
        const string raw = properties.get(key, "");
        const double value = raw.empty() ? fallback : ScenarioParser::parseDouble(raw, fallback);
        spec.knobs[key] = std::to_string(value);
        return value;
    }

    string knobString(const BillProperties &properties, ScenarioSpec &spec, const string &key, const string &fallback) {
        const string value = properties.get(key, fallback);
        spec.knobs[key] = value;
        return value;
    }

    //! Splits "pos=1,2,3; n=500" into its key/value pieces.
    std::map<string, string> parseGroupLine(const string &line) {
        std::map<string, string> fields;
        std::stringstream stream(line);
        string token;
        while (getline(stream, token, ';')) {
            const auto equals = token.find('=');
            if (equals == string::npos) {
                continue;
            }
            fields[BillProperties::trim(token.substr(0, equals))] =
                    BillProperties::trim(token.substr(equals + 1));
        }
        return fields;
    }

    string field(const std::map<string, string> &fields, const string &key) {
        const auto found = fields.find(key);
        return found == fields.end() ? "" : found->second;
    }

}

vector<GroupSpec> ScenarioParser::explicitGroups(const BillProperties &properties, const GroupSpec &defaults) {
    vector<GroupSpec> groups;

    for (int i = 0; ; i++) {
        const string key = "group." + std::to_string(i);
        if (!properties.has(key)) {
            break;
        }

        const auto fields = parseGroupLine(properties.at(key.c_str()));

        GroupSpec group = defaults;
        group.label = field(fields, "label").empty() ? key : field(fields, "label");
        group.position = parseVector(field(fields, "pos"), defaults.position);
        group.velocity = parseVector(field(fields, "vel"), defaults.velocity);
        group.spin = parseVector(field(fields, "spin"), defaults.spin);
        group.count = (int) parseDouble(field(fields, "n"), defaults.count);
        group.mass = parseDouble(field(fields, "mass"), defaults.mass);
        group.radius = parseDouble(field(fields, "radius"), defaults.radius);
        group.dispersion = parseDouble(field(fields, "dispersion"), defaults.dispersion);
        group.color = field(fields, "color").empty()
                      ? paletteColor(i)
                      : parseVector(field(fields, "color"), defaults.color);

        groups.push_back(group);
    }

    if (groups.empty()) {
        std::cout << "scenario.type=explicit but no group.N lines were found." << std::endl;
        exit(1);
    }

    return groups;
}

vector<GroupSpec> ScenarioParser::cluster(const BillProperties &properties, const GroupSpec &defaults, ScenarioSpec &spec) {
    GroupSpec group = defaults;
    group.label = "cluster";
    group.radius = knob(properties, spec, "cluster.radius", defaults.radius);
    group.spin = parseVector(knobString(properties, spec, "cluster.spin", "0"), PhysicalVector(0, 0, 0, false));
    group.dispersion = knob(properties, spec, "cluster.dispersion", 0.2);
    group.count = (int) knob(properties, spec, "cluster.count", defaults.count);
    group.mass = knob(properties, spec, "cluster.mass", defaults.mass);
    group.color = parseVector(knobString(properties, spec, "cluster.color", "1,1,1"), PhysicalVector(1, 1, 1));

    return {group};
}

/*! Two groups on a collision course.
 *
 *  The knobs are dimensionless so they can be sampled blindly and still land
 *  somewhere sensible:
 *   - pair.mass_ratio        heavier group over lighter group
 *   - pair.separation        centre-to-centre start distance, in meters
 *   - pair.impact_parameter  perpendicular offset as a fraction of (R1 + R2).
 *                            0 is head-on, ~0.5-1.0 grazes and makes tidal tails
 *   - pair.approach_speed    closing speed as a fraction of mutual escape speed.
 *                            <1 is bound, ~1 is parabolic, >1 is a flyby
 */
vector<GroupSpec> ScenarioParser::pair(const BillProperties &properties, const GroupSpec &defaults, ScenarioSpec &spec) {
    const double massRatio = std::max(1.0, knob(properties, spec, "pair.mass_ratio", 1.0));
    const double separation = knob(properties, spec, "pair.separation", defaults.radius * 6.0);
    const double impactFraction = knob(properties, spec, "pair.impact_parameter", 0.4);
    const double approachFraction = knob(properties, spec, "pair.approach_speed", 0.6);
    const double spinA = knob(properties, spec, "pair.spin_a", 0.3);
    const double spinB = knob(properties, spec, "pair.spin_b", 0.3);
    const double dispersion = knob(properties, spec, "pair.dispersion", 0.15);

    const double totalMass = defaults.mass * 2.0;
    const double massA = totalMass * massRatio / (massRatio + 1.0);
    const double massB = totalMass - massA;

    // Radius scales with the cube root of mass so both groups keep one density.
    const double radiusA = defaults.radius * cbrt(massA / defaults.mass);
    const double radiusB = defaults.radius * cbrt(massB / defaults.mass);

    const int totalCount = defaults.count * 2;
    int countA = (int) (totalCount * massA / totalMass);
    countA = std::max(1, std::min(totalCount - 1, countA));
    const int countB = totalCount - countA;

    const double impact = impactFraction * (radiusA + radiusB);
    const double escapeSpeed = sqrt(2.0 * physics_constants::G * totalMass / std::max(separation, 1.0));
    const double closingSpeed = approachFraction * escapeSpeed;

    // Split the closing speed so the pair's centre of mass stays put.
    const double speedA = closingSpeed * massB / totalMass;
    const double speedB = closingSpeed * massA / totalMass;

    GroupSpec a = defaults;
    a.label = "primary";
    a.mass = massA;
    a.radius = radiusA;
    a.count = countA;
    a.dispersion = dispersion;
    a.position = PhysicalVector((float) (-separation / 2.0), (float) (impact / 2.0), 0, false);
    a.velocity = PhysicalVector((float) speedA, 0, 0, false);
    a.spin = PhysicalVector(0, 0, (float) spinA, false);
    a.color = paletteColor(0);

    GroupSpec b = defaults;
    b.label = "secondary";
    b.mass = massB;
    b.radius = radiusB;
    b.count = countB;
    b.dispersion = dispersion;
    b.position = PhysicalVector((float) (separation / 2.0), (float) (-impact / 2.0), 0, false);
    b.velocity = PhysicalVector((float) -speedB, 0, 0, false);
    b.spin = PhysicalVector(0, 0, (float) spinB, false);
    b.color = paletteColor(2);

    return {a, b};
}

/*! N groups scattered in a sphere, each falling toward the centre.
 *
 *  This is the archetype to sample when you want variety per seed.
 */
vector<GroupSpec> ScenarioParser::chaotic(
        const BillProperties &properties, const GroupSpec &defaults, ScenarioSpec &spec, std::mt19937 &rng) {
    const int groupCount = (int) knob(properties, spec, "chaotic.groups", 6);
    const double spread = knob(properties, spec, "chaotic.spread", defaults.radius * 8.0);
    const double sizeFraction = knob(properties, spec, "chaotic.size_fraction", 0.18);
    const double infall = knob(properties, spec, "chaotic.infall", 0.5);
    const double tangential = knob(properties, spec, "chaotic.tangential", 0.35);
    const double spinMax = knob(properties, spec, "chaotic.spin_max", 0.4);
    const double massSpread = knob(properties, spec, "chaotic.mass_spread", 2.0);
    const double dispersion = knob(properties, spec, "chaotic.dispersion", 0.15);
    const double thickness = knob(properties, spec, "chaotic.thickness", 0.25);

    std::uniform_real_distribution<double> unit(0.0, 1.0);
    std::uniform_real_distribution<double> signed_(-1.0, 1.0);

    // Mass of the whole system, so infall speeds can be scaled against it.
    const double systemMass = defaults.mass * groupCount;

    vector<GroupSpec> groups;
    for (int i = 0; i < groupCount; i++) {
        GroupSpec group = defaults;
        group.label = "chaotic." + std::to_string(i);

        const double massFactor = pow(massSpread, signed_(rng));
        group.mass = defaults.mass * massFactor;
        group.count = std::max(1, (int) (defaults.count * massFactor));
        group.radius = spread * sizeFraction * cbrt(massFactor);

        // Placed in a flattened sphere so the action reads well in the XY view.
        const double theta = unit(rng) * 2.0 * M_PI;
        const double radial = spread * cbrt(unit(rng));
        const double x = radial * cos(theta);
        const double y = radial * sin(theta);
        const double z = signed_(rng) * spread * thickness;
        group.position = PhysicalVector((float) x, (float) y, (float) z, false);

        const double distance = std::max(sqrt(x * x + y * y + z * z), 1.0);
        const double escapeSpeed = sqrt(2.0 * physics_constants::G * systemMass / distance);

        // Inward component, built from the guarded distance so a group that
        // lands on the centre doesn't divide by zero.
        const PhysicalVector inward = PhysicalVector(
                (float) (-x / distance), (float) (-y / distance), (float) (-z / distance), false)
                .scaledBy(infall * escapeSpeed);

        // Tangential kick, mostly one way round so the system as a whole spins,
        // with the occasional retrograde group to break the symmetry up.
        const double inPlane = sqrt(x * x + y * y);
        PhysicalVector tangent(0, 0, 0, false);
        if (inPlane > 0) {
            const double direction = unit(rng) > 0.15 ? 1.0 : -1.0;
            tangent = PhysicalVector((float) (-y / inPlane), (float) (x / inPlane), 0, false)
                    .scaledBy(tangential * escapeSpeed * direction);
        }

        group.velocity = inward.plus(tangent);

        group.spin = PhysicalVector(0, 0, (float) (signed_(rng) * spinMax), false);
        group.dispersion = dispersion;
        group.color = paletteColor(i);

        groups.push_back(group);
    }

    return groups;
}

/*! A ring of groups all swinging past the centre - the parameterized version of
 *  the old hard-coded fourInADiamond().
 */
vector<GroupSpec> ScenarioParser::diamond(const BillProperties &properties, const GroupSpec &defaults, ScenarioSpec &spec) {
    const int groupCount = (int) knob(properties, spec, "diamond.groups", 8);
    const double ringRadius = knob(properties, spec, "diamond.ring_radius", defaults.radius * 5.0);
    const double infall = knob(properties, spec, "diamond.infall", 0.5);
    const double swirl = knob(properties, spec, "diamond.swirl", 0.35);
    const double spin = knob(properties, spec, "diamond.spin", 0.25);
    const double dispersion = knob(properties, spec, "diamond.dispersion", 0.1);

    const double systemMass = defaults.mass * groupCount;
    const double escapeSpeed = sqrt(2.0 * physics_constants::G * systemMass / std::max(ringRadius, 1.0));

    vector<GroupSpec> groups;
    for (int i = 0; i < groupCount; i++) {
        const double angle = 2.0 * M_PI * i / groupCount;
        const double x = ringRadius * cos(angle);
        const double y = ringRadius * sin(angle);

        GroupSpec group = defaults;
        group.label = "ring." + std::to_string(i);
        group.position = PhysicalVector((float) x, (float) y, 0, false);
        group.velocity = PhysicalVector(
                (float) (-infall * escapeSpeed * cos(angle) - swirl * escapeSpeed * sin(angle)),
                (float) (-infall * escapeSpeed * sin(angle) + swirl * escapeSpeed * cos(angle)),
                0, false);
        group.spin = PhysicalVector(0, 0, (float) spin, false);
        group.dispersion = dispersion;
        group.color = paletteColor(i);

        groups.push_back(group);
    }

    return groups;
}

ScenarioSpec ScenarioParser::parse(
        const BillProperties &properties,
        const PhysicsSandboxProperties &sandboxProperties,
        unsigned int seed) {

    ScenarioSpec spec;
    spec.seed = seed;
    spec.type = properties.get("scenario.type", "diamond");
    spec.name = properties.get("scenario.name", spec.type);
    spec.knobs["scenario.type"] = spec.type;
    spec.knobs["scenario.name"] = spec.name;

    // numShapes and mass keep their old meaning, but now they are per-group
    // defaults that any archetype or group line may override.
    GroupSpec defaults;
    defaults.count = sandboxProperties.numShapes;
    defaults.mass = sandboxProperties.mass.value() * 1000.0;
    defaults.radius = sandboxProperties.sandboxWidth;

    spec.virialRatio = knob(properties, spec, "scenario.virial_ratio", -1.0);

    std::mt19937 rng(seed);

    if (spec.type == "explicit") {
        spec.groups = explicitGroups(properties, defaults);
    } else if (spec.type == "cluster") {
        spec.groups = cluster(properties, defaults, spec);
    } else if (spec.type == "pair") {
        spec.groups = pair(properties, defaults, spec);
    } else if (spec.type == "chaotic") {
        spec.groups = chaotic(properties, defaults, spec, rng);
    } else if (spec.type == "diamond") {
        spec.groups = diamond(properties, defaults, spec);
    } else {
        std::cout << "Unknown scenario.type: " << spec.type
                  << " (expected explicit, cluster, pair, chaotic or diamond)" << std::endl;
        exit(1);
    }

    return spec;
}
