#include "PhysicsSolver.h"

void PhysicsSolver::solve(float dt) {
    const float timeStep = dt / m_iterationCount;
    for (const auto object : m_objects) object->applyForces(dt);

    for (auto i : CountIterator(m_iterationCount)) solveIteration(timeStep);
}

void PhysicsSolver::solveIteration(float dt) {

}
