#include "PhysicsSolver.h"

#include <algorithm>
#include <cmath>

#include "core/checks.h"
#include "core/Engine.h"
#include "core/GraphicsEngine.h"


sf::Angle getTransformRotation(const sf::Transform& transform) {
    const float* m = transform.getMatrix();
    return sf::radians(std::atan2(m[1], m[0]));
}

// TODO: constraint friction forces
void PhysicsSolver::solve(float dt) {
    check(m_iterationCount > 0);
    if (m_iterationCount <= 0)
        return;

    const float timeStep = dt / static_cast<float>(m_iterationCount);

    for (const auto object : m_objects)
        object->applyForces(dt, true);

    for (auto i : CountIterator(m_iterationCount))
        solveIteration(timeStep);
}

void PhysicsSolver::tick(float deltaTime) {
    m_accumulator += deltaTime;
    if (m_accumulator > 1.f) m_accumulator = 1.f; //cap how much it can lag behind

    while (m_accumulator >= m_physicsDt) {
        solve(m_physicsDt);
        m_accumulator -= m_physicsDt;
    }

    if (!draw)
        return;

    for (const auto body : m_objects)
        g_engine.getGraphicsEngine()->render(*body);

    for (const auto constraint : m_constraints)
        g_engine.getGraphicsEngine()->render(*constraint);
}

PhysicsSolver::CorrectionCache::CorrectionCache(Constraint* constraint) {
    const auto* sourceObject = constraint->sourceObject;
    const auto* targetObject = constraint->targetObject;

    check(targetObject);
    // A target object is required; a null source represents the world.

    staticSource = !sourceObject || sourceObject->isStatic();

    sourceInvMass = 0.f;
    sourceInvInertia = 0.f;

    if (!staticSource) {
        check(sourceObject->getMass() > 0.f);
        check(sourceObject->getMomentOfInertia() > 0.f);

        sourceInvMass = 1.f / sourceObject->getMass();
        sourceInvInertia = 1.f / sourceObject->getMomentOfInertia();
    }

    targetInvMass = 0.f;
    targetInvInertia = 0.f;

    if (!targetObject->isStatic()) {
        check(targetObject->getMass() > 0.f);
        check(targetObject->getMomentOfInertia() > 0.f);

        targetInvMass = 1.f / targetObject->getMass();
        targetInvInertia = 1.f / targetObject->getMomentOfInertia();
    }

    sourceTransform = constraint->getSourceWorldTransform();
    targetTransform = constraint->getTargetWorldTransform();

    sourceAnchor = sourceTransform.transformPoint({0.f, 0.f});
    targetAnchor = targetTransform.transformPoint({0.f, 0.f});

    // Local COM coordinates belong to the body, not its attachment.
    sourceCentreOfMass = sourceObject
        ? sourceObject->getTransform().transformPoint(
            sourceObject->getLocalCenterOfMass())
        : sourceAnchor;

    targetCentreOfMass =
        targetObject->getTransform().transformPoint(
            targetObject->getLocalCenterOfMass());

    rs = sourceAnchor - sourceCentreOfMass;
    rt = targetAnchor - targetCentreOfMass;
}

void PhysicsSolver::solveIteration(float /*dt*/) {
    for (const auto constraint : m_constraints) {
        check(constraint);
        if (!constraint)
            continue;

        check(constraint->targetObject);
        if (!constraint->targetObject)
            continue;

        const auto& xConstraint =
            constraint->dofConstraints[DoFConstraint::X];

        const auto& yConstraint =
            constraint->dofConstraints[DoFConstraint::Y];

        const auto& thetaConstraint =
            constraint->dofConstraints[DoFConstraint::Theta];

        // Recompute both the error and geometry for each axis:
        // correcting X can also change Y and the relative rotation.
        const auto solvePositionAxis = [&](bool isX) {
            const auto& axisConstraint = isX ? xConstraint : yConstraint;

            if (!axisConstraint.active)
                return;

            check(axisConstraint.lowerBound <= axisConstraint.upperBound);
            if (axisConstraint.lowerBound > axisConstraint.upperBound)
                return;

            const CorrectionCache cache(constraint);

            const sf::Transform relativeTransform =
                cache.sourceTransform.getInverse() * cache.targetTransform;

            const sf::Vector2f relativePosition =
                relativeTransform.transformPoint({0.f, 0.f});

            const float position = isX
                ? relativePosition.x
                : relativePosition.y;

            const float desiredPosition = std::clamp(
                position,
                axisConstraint.lowerBound,
                axisConstraint.upperBound);

            const float correction = desiredPosition - position;

            if (correction != 0.f)
                applyPositionCorrection(correction, constraint, isX, cache);
        };

        solvePositionAxis(true);
        solvePositionAxis(false);

        if (thetaConstraint.active) {
            const CorrectionCache cache(constraint);

            const sf::Transform relativeTransform =
                cache.sourceTransform.getInverse() * cache.targetTransform;

            const sf::Angle relativeRotation =
                getTransformRotation(relativeTransform);

            sf::Angle correction = sf::Angle::Zero;

            // Assumes an ordered interval on the signed angle branch,
            // with no crossing of the -pi / +pi boundary.
            if (relativeRotation < thetaConstraint.minAngle) {
                correction = thetaConstraint.minAngle - relativeRotation;
            } else if (relativeRotation > thetaConstraint.maxAngle) {
                correction = thetaConstraint.maxAngle - relativeRotation;
            }

            if (correction != sf::Angle::Zero)
                applyRotationCorrection(correction, constraint, cache);
        }
    }
}

void PhysicsSolver::applyPositionCorrection(
    float correction,
    Constraint* constraint,
    bool isX,
    const CorrectionCache& cache)
{
    // Assumes rigid transforms: translation and rotation, no scaling.
    const sf::Vector2f localAxis = isX
        ? sf::Vector2f{1.f, 0.f}
        : sf::Vector2f{0.f, 1.f};

    const sf::Vector2f n =
        cache.sourceTransform.transformPoint(localAxis) -
        cache.sourceTransform.transformPoint({0.f, 0.f});

    // The constraint axis rotates with the source body.
    // Its exact angular Jacobian therefore also includes the
    // separation between the attachment points.
    const sf::Vector2f separation =
        cache.targetAnchor - cache.sourceAnchor;

    const float sourceLever = (cache.rs + separation).cross(n);
    const float targetLever = cache.rt.cross(n);

    const float w =
        cache.sourceInvMass +
        cache.targetInvMass +
        cache.sourceInvInertia * sourceLever * sourceLever +
        cache.targetInvInertia * targetLever * targetLever;

    if (w <= 0.f)
        return;

    const float lambda = correction / w;
    const sf::Vector2f P = lambda * n;

    const sf::Vector2f sourcePositionDelta =
        -cache.sourceInvMass * P;

    const sf::Vector2f targetPositionDelta =
        cache.targetInvMass * P;

    const float sourceAngleDelta =
        -cache.sourceInvInertia * sourceLever * lambda;

    const float targetAngleDelta =
        cache.targetInvInertia * targetLever * lambda;

    applyBodyCorrection(
        constraint->sourceObject,
        sourcePositionDelta,
        sourceAngleDelta);

    applyBodyCorrection(
        constraint->targetObject,
        targetPositionDelta,
        targetAngleDelta);
}

void PhysicsSolver::applyRotationCorrection(
    sf::Angle correction,
    Constraint* constraint,
    const CorrectionCache& cache)
{
    const float angularW =
        cache.sourceInvInertia + cache.targetInvInertia;

    if (angularW <= 0.f)
        return;

    const float lambdaAngle = correction.asRadians() / angularW;

    const float sourceAngleDelta =
        -cache.sourceInvInertia * lambdaAngle;

    const float targetAngleDelta =
        cache.targetInvInertia * lambdaAngle;

    applyBodyCorrection(
        constraint->sourceObject,
        {0.f, 0.f},
        sourceAngleDelta);

    applyBodyCorrection(
        constraint->targetObject,
        {0.f, 0.f},
        targetAngleDelta);
}

void PhysicsSolver::applyBodyCorrection(PhysicsObject *body, sf::Vector2f positionDelta, float angleDelta) {
    if (!body || body->isStatic())
        return;

    const sf::Vector2f localCOM = body->getLocalCenterOfMass();

    const sf::Vector2f desiredCOM =
            body->getTransform().transformPoint(localCOM) + positionDelta;

    body->rotate(sf::radians(angleDelta));

    // Compensate for the SFML origin potentially differing from the COM.
    const sf::Vector2f currentCOM =
            body->getTransform().transformPoint(localCOM);

    body->move(desiredCOM - currentCOM);

    body->recalculateCenterOfMass();
}
