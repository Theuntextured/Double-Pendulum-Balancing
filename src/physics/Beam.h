#pragma once
#include "PhysicsCore.h"

class Beam : public PhysicsObject {
public:
    float getMomentOfInertia() const override;
    sf::Vector2f getLocalCenterOfMass() const override;

private:
    float length{1};
};
