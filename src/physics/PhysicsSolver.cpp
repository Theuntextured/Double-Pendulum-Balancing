#include "PhysicsSolver.h"

#include "core/Engine.h"
#include "core/GraphicsEngine.h"

void PhysicsSolver::solve(float dt) {
    const float timeStep = dt / static_cast<float>(m_iterationCount);

    for (const auto object : m_objects) object->applyForces(dt, true);

    //apply friction forces due to constraints.
    //Treated separately to other forces since resistive forces should not accelerate back (forcing oscillations)
    for (const auto constraint : m_constraints) {
        assert(constraint->targetObject);
        // TODO

        /*
        for (size_t i = 0; i < DegreeOfFreedomConstraint::Num; i++) {
            const auto &dof = constraint->dofConstraints[i];
            if (!dof.active) continue;

            switch (static_cast<DegreeOfFreedomConstraint::Type>(i)) {
                case DegreeOfFreedomConstraint::X:
                    const auto axis = sf::Vector2f(1, 0).rotatedBy(sf::degrees())
                    float F = dof.linearDamping *
                    break;
                case DegreeOfFreedomConstraint::Y:
                    break;
                case DegreeOfFreedomConstraint::Theta:
                    break;
                default:
                    LOG(Physics, Fatal, "Invalid constraint target DOF.");
                    assert(false);
            }
        }
        */

    }

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

}
