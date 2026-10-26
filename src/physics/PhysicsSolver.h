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
    void solveIteration(float dt);
};
