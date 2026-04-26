#ifndef CLOSEST_PAIR_TRACKER_H
#define CLOSEST_PAIR_TRACKER_H

#include "Particle.h"
#include <vector>
#include <algorithm>
#include <memory>

using std::shared_ptr;

// Tracks the N closest pairs of particles during octree traversal
// Thread-safe for parallel use with external mutex
class ClosestPairTracker {
public:
    explicit ClosestPairTracker(int maxPairs) : maxPairs(maxPairs) {
        if (maxPairs > 0) {
            pairs.reserve(maxPairs);
        }
    }

    // Consider a pair during traversal - will be kept if among N closest
    void consider(shared_ptr<Particle> a, shared_ptr<Particle> b, double distance) {
        if (maxPairs <= 0) return;

        if (pairs.size() < static_cast<size_t>(maxPairs)) {
            pairs.push_back({a, b, distance});
            // Keep sorted by distance (largest first for easy removal)
            std::sort(pairs.begin(), pairs.end(),
                [](const PairWithDistance& x, const PairWithDistance& y) {
                    return x.distance > y.distance;
                });
        } else if (distance < pairs.front().distance) {
            // Replace the largest distance with this smaller one
            pairs.front() = {a, b, distance};
            std::sort(pairs.begin(), pairs.end(),
                [](const PairWithDistance& x, const PairWithDistance& y) {
                    return x.distance > y.distance;
                });
        }
    }

    // Merge another tracker's results (for combining parallel results)
    void merge(const ClosestPairTracker& other) {
        for (const auto& pair : other.pairs) {
            consider(pair.a, pair.b, pair.distance);
        }
    }

    // Get the closest pairs found
    std::vector<std::pair<shared_ptr<Particle>, shared_ptr<Particle>>> getPairs() const {
        std::vector<std::pair<shared_ptr<Particle>, shared_ptr<Particle>>> result;
        result.reserve(pairs.size());
        for (const auto& pwd : pairs) {
            result.push_back({pwd.a, pwd.b});
        }
        return result;
    }

    bool isTracking() const { return maxPairs > 0; }

private:
    struct PairWithDistance {
        shared_ptr<Particle> a;
        shared_ptr<Particle> b;
        double distance;
    };

    int maxPairs;
    std::vector<PairWithDistance> pairs;
};

#endif
