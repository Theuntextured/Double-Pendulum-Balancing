#pragma once
#include "core/Common.h"

class PhysicsObject : public sf::Transformable {
public:
    friend class PhysicsSolver;

    void Init();

    virtual float getMomentOfInertia() const = 0;
    virtual sf::Vector2f getLocalCenterOfMass() const = 0;


    void addForce(sf::Vector2f force, bool localSpace = false);
    void addForce(sf::Vector2f force, sf::Vector2f point, 
        bool forceLocalSpace = false, bool pointLocalSpace = false);
    void addTorque(float torque);

    void applyImpulse(sf::Vector2f impulse, bool localSpace = false);
    void applyImpulse(sf::Vector2f impulse, sf::Vector2f point,
        bool forceLocalSpace = false, bool pointLocalSpace = false);
    void applyTorqueImpulse(float torqueImpulse);
    
    float getMass() const { return m_mass; }
    float getAngularVelocity() const { return m_angularVelocity; }
    sf::Vector2f getVelocity() const { return m_velocity; }
    bool getIsStatic() const { return m_isStatic; }
    bool getIsInitialized() const { return m_isInitialized; }

    void setMass(float mass) { m_mass = mass; recalculateMomentOfInertia(); }

    void recalculateCenterOfMass();
    void recalculateMomentOfInertia();

private:
    void applyForces(float deltaTime);
    void resetForces();
    sf::Vector2f m_accumulatedForce{0, 0};
    float m_accumulatedTorque = 0;
    sf::Vector2f m_accumulatedImpulse{0, 0};
    float m_accumulatedTorqueImpulse = 0;

    float m_mass = 1;
    sf::Vector2f m_velocity;
    float m_angularVelocity{0};
    bool m_isStatic{false};
    bool m_isInitialized{false};

    float m_cachedMomentOfInertia = 1;
    sf::Vector2f m_cachedLocalCenterOfMass = {0, 0};
    sf::Vector2f m_cachedGlobalCenterOfMass = {0, 0};
};
