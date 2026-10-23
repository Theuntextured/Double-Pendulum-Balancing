#pragma once
#include <vector>

#include "Constraint.h"
#include "PhysicsCore.h"

class PhysicsSolver {
    size_t m_iterationCount = 3;
    std::vector<PhysicsObject*> m_objects;
    std::vector<Constraint*> m_constraints;
public:
    void solve(float dt);

private:
    void solveIteration(float dt);
};
