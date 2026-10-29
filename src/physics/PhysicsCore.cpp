#include "PhysicsCore.h"

#include "PhysicsSolver.h"

DEFINE_LOG_GROUP(Physics, Warning);

void PhysicsObject::init(class PhysicsSolver& solver) {
    recalculateCenterOfMass();
    recalculateMomentOfInertia();
    m_isInitialized = true;

    solver.m_objects.emplace(this);
}

void PhysicsObject::addForce(sf::Vector2f force, bool localSpace) {
    if (localSpace) force = force.rotatedBy(getRotation());
    m_accumulatedForce += force;
}

void PhysicsObject::addForce(sf::Vector2f force, sf::Vector2f point,
    bool forceLocalSpace, bool pointLocalSpace) {
    if (forceLocalSpace) force = force.rotatedBy(getRotation());
    if (pointLocalSpace) point = getTransform().transformPoint(point);

    m_accumulatedForce += force;

    const auto r = point - m_cachedGlobalCenterOfMass;
    m_accumulatedTorque += r.x * force.y - r.y * force.x;
}

void PhysicsObject::addTorque(float torque) {
    m_accumulatedTorque += torque;
}

void PhysicsObject::applyImpulse(sf::Vector2f impulse, bool localSpace) {
    if (localSpace) impulse = impulse.rotatedBy(getRotation());
    m_accumulatedImpulse += impulse;
}

void PhysicsObject::applyImpulse(sf::Vector2f impulse, sf::Vector2f point,
    bool forceLocalSpace, bool pointLocalSpace) {
    if (forceLocalSpace) impulse = impulse.rotatedBy(getRotation());
    if (pointLocalSpace) point = getTransform().transformPoint(point);
    m_accumulatedImpulse += impulse;

    const auto r = point - m_cachedGlobalCenterOfMass;
    m_accumulatedTorqueImpulse += r.x * impulse.y - r.y * impulse.x;
}

void PhysicsObject::applyTorqueImpulse(float torqueImpulse) {
    m_accumulatedTorqueImpulse += torqueImpulse;
}

void PhysicsObject::recalculateCenterOfMass() {
    m_cachedLocalCenterOfMass = getLocalCenterOfMass();
    m_cachedGlobalCenterOfMass = getTransform().transformPoint(m_cachedLocalCenterOfMass);
}

void PhysicsObject::recalculateMomentOfInertia() {
    m_cachedMomentOfInertia = getMomentOfInertia();
}

void PhysicsObject::applyForces(float deltaTime, bool applyGravity) {
    if (deltaTime <= 0) return;
    if (!m_isStatic) {
        auto velocity = (getPosition() - m_previousPosition) / deltaTime;
        auto angularVelocity = (getRotation() - m_previousRotation) / deltaTime;

        if (applyGravity) m_accumulatedForce.y -= 981 * deltaTime;
        velocity += (m_accumulatedForce * deltaTime + m_accumulatedImpulse) / m_mass;
        angularVelocity += sf::radians((m_accumulatedTorque * deltaTime + m_accumulatedTorqueImpulse) / m_cachedMomentOfInertia);

        m_previousPosition = getPosition();
        m_previousRotation = getRotation();

        setPosition(getPosition()      + velocity        * deltaTime);
        setRotation(getRotation() + angularVelocity * deltaTime);
    }
    resetForces();
}

void PhysicsObject::resetForces() {
    m_accumulatedForce = {0, 0};
    m_accumulatedImpulse = {0, 0};
    m_accumulatedTorqueImpulse = 0;
    m_accumulatedTorque = 0;
}

