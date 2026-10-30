#pragma once
#include "PhysicsCore.h"

class PointMass : public PhysicsObject {
public:
    float getMomentOfInertia() const override { return INFINITY; } // 0 would break shit
    sf::Vector2f getLocalCenterOfMass() const override { return {0, 0}; }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
