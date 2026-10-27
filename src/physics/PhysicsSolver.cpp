#include "PhysicsSolver.h"

#include "core/checks.h"
#include "core/Engine.h"
#include "core/GraphicsEngine.h"

// TODO: constraint friction forces
void PhysicsSolver::solve(float dt) {
    const float timeStep = dt / static_cast<float>(m_iterationCount);

    for (const auto object : m_objects) object->applyForces(dt, true);

    for (auto i : CountIterator(m_iterationCount)) solveIteration(timeStep);
}

void PhysicsSolver::tick(float deltaTime) {
    solve(deltaTime);

    if (!draw) return;

    for (const auto body : m_objects)
        g_engine.getGraphicsEngine()->render(*body);
    for (const auto constraint : m_constraints)
        g_engine.getGraphicsEngine()->render(*constraint);
}

sf::Angle getTransformRotation(const sf::Transform& transform) {
    const float* m = transform.getMatrix();
    return sf::radians(std::atan2(m[1], m[0]));
}

void PhysicsSolver::solveIteration(float dt) {
    for (const auto constraint : m_constraints) {
        check(!constraint->targetObject->isStatic());
        const sf::Transform targetRelativeTransform = constraint->getTargetRelativeTransform();

        auto& sourceObject = *constraint->sourceObject;
        auto& targetObject = *constraint->targetObject;

        const auto& xConstraint = constraint->dofConstraints[DoFConstraint::X];
        const auto& yConstraint = constraint->dofConstraints[DoFConstraint::Y];
        const auto& thetaConstraint = constraint->dofConstraints[DoFConstraint::Theta];


        sf::Vector2f correction{0,0};
        sf::Angle angleCorrection = sf::Angle::Zero;
        const float alpha = constraint->sourceObject && !constraint->sourceObject->isStatic()
            ? targetObject.getMass() / (targetObject.getMass() + sourceObject.getMass())
            : 1.f;
        const float angleAlpha = constraint->sourceObject && !constraint->sourceObject->isStatic()
            ? targetObject.getMomentOfInertia() / (targetObject.getMomentOfInertia() + sourceObject.getMomentOfInertia())
            : 1.f;

        if (xConstraint.active || yConstraint.active) {
            const sf::Vector2f relativePosition = targetRelativeTransform.transformPoint({0,0});
            if (xConstraint.active) {
                if (relativePosition.x < xConstraint.lowerBound)
                    correction.x = xConstraint.lowerBound - relativePosition.x;
                else if (relativePosition.x > xConstraint.upperBound)
                    correction.x = xConstraint.upperBound - relativePosition.x;
            }
            if (yConstraint.active) {
                if (relativePosition.y < yConstraint.lowerBound)
                    correction.y = yConstraint.lowerBound - relativePosition.y;
                else if (relativePosition.y > yConstraint.upperBound)
                    correction.y = yConstraint.upperBound - relativePosition.y;
            }
        }
        if (thetaConstraint.active) {
            const sf::Angle relativeRotation = getTransformRotation(targetRelativeTransform);
            if (relativeRotation < thetaConstraint.minAngle)
                angleCorrection = thetaConstraint.minAngle - relativeRotation;
            else if (relativeRotation > thetaConstraint.maxAngle)
                angleCorrection = thetaConstraint.maxAngle - relativeRotation;
        }



    }
}
