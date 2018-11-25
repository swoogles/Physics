//
// Created by bfrasure on 8/10/18.
//

#include "ComplicatedFunctions.h"

ParticleList ComplicatedFunctions::nonCollidingObjects(ParticleList allShapes, PairCollection collidingObjects) {
    // YES! Ugly as fuck, but this is generally what I need.
    auto shapeVec = allShapes.getShapes();
    std::vector<shared_ptr<Circle>> nonCollidingObjects(allShapes.size());
    auto brittlePairs = collidingObjects.brittlePairs();
    auto it = std::copy_if(
            shapeVec.begin(),
            shapeVec.end(),
            nonCollidingObjects.begin(),
            [brittlePairs](const auto &circle) {
                return !std::any_of(
                        brittlePairs.begin(),
                        brittlePairs.end(),
                        // TODO any way to use method reference here?
                        [circle](const TouchingPair &pair) {
                            Circle & circleRef = *circle;
                            return pair.contains(circleRef);
                        });
            });

    nonCollidingObjects.resize(std::distance(nonCollidingObjects.begin(), it));

    ParticleList nonColliders(nonCollidingObjects);
    return nonColliders;
}
