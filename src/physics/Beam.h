#pragma once
#include "PhysicsCore.h"

class Beam : public PhysicsObject {
public:
    Beam(float length, float mass);
    float getMomentOfInertia() const override;
    sf::Vector2f getLocalCenterOfMass() const override;

    float getLength() const { return m_length; }

private:
    float m_length{1};

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
