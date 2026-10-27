#include "PhysicsSolver.h"

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

void PhysicsSolver::solveIteration(float dt) {
    for (const auto constraint : m_constraints) {

    }
}
