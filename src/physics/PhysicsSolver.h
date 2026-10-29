#pragma once
#include <unordered_set>

#include "Constraint.h"
#include "PhysicsCore.h"

class PhysicsSolver : public Tickable {
    friend class PhysicsObject;
    friend class Constraint;

    size_t m_iterationCount = 3;
    std::unordered_set<PhysicsObject*> m_objects;
    std::unordered_set<Constraint*> m_constraints;
public:
    void solve(float dt);

    void tick(float deltaTime) override;

    bool draw = false;

private:
    struct CorrectionCache {
        explicit CorrectionCache(Constraint* constraint);

        bool staticSource;

        float sourceInvMass;
        float targetInvMass;

        float sourceInvInertia;
        float targetInvInertia;

        sf::Transform sourceTransform;
        sf::Transform targetTransform;

        sf::Vector2f sourceAnchor;
        sf::Vector2f targetAnchor;

        sf::Vector2f sourceCentreOfMass;
        sf::Vector2f targetCentreOfMass;

        sf::Vector2f rs;
        sf::Vector2f rt;
    };

    void solveIteration(float dt);

    static void applyPositionCorrection(float correction, Constraint* constraint, bool isX, const CorrectionCache& cache);

    static void applyRotationCorrection(sf::Angle correction, Constraint* constraint, const CorrectionCache& cache);

    static void applyBodyCorrection(
    PhysicsObject* body,
    sf::Vector2f positionDelta,
    float angleDelta);

private:
    float m_accumulator = 0;
    float m_physicsDt = 1 / 120.f;
};
