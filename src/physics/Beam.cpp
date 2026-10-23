#include "Beam.h"

float Beam::getMomentOfInertia() const {
    return getMass() * length * length / 12;
}

sf::Vector2f Beam::getLocalCenterOfMass() const {
    return {length * 0.5f, 0};
}

