#pragma once
#include <atomic>
#include <unordered_set>

#include "Constraint.h"
#include "PhysicsCore.h"

class PhysicsSolver : public Tickable {
    friend class PhysicsObject;
    friend class Constraint;

    size_t m_iterationCount = 3;
    std::unordered_set<PhysicsObject*> m_objects;
    std::unordered_set<Constraint*> m_constraints;

    static std::atomic_uint64_t s_solverCount;
    size_t m_solverId;
public:
    PhysicsSolver();
    void solve(float dt);

    void tick(float deltaTime) override;

    bool draw = false;

    Constraint* getConstraintAt(sf::Vector2f location, float tolerance = 50.f) const;

    void setDragTarget(Constraint* constraint, sf::Vector2f target, float strength);
    void clearDragTarget() { m_draggedConstraint = nullptr; }

private:
    Constraint* m_draggedConstraint{nullptr};
    sf::Vector2f m_dragTarget;
    float m_dragStrength = 0.f;

    void applyDragForce(float dt) const;

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
