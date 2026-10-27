#include "physics/Beam.h"
#include "physics/PhysicsSolver.h"
#include "src/core/Engine.h"

int main()
{
    g_engine.init();

    PhysicsSolver solver;

    Beam firstRod(300.f, 1.f);
    Beam secondRod(500.f, 1.5f);
    Constraint rail;
    auto& railYConstraint = rail.dofConstraints[DoFConstraint::X];
    railYConstraint.active = true;
    railYConstraint.lowerBound = -600;
    railYConstraint.upperBound = 600;
    rail.dofConstraints[DoFConstraint::Y].active = true;
    rail.sourceTransform.translate({800, 128.f});
    rail.targetObject = &firstRod;
    rail.targetTransform.translate({-firstRod.getLength() / 2, 0});

    Constraint internal;
    internal.dofConstraints[DoFConstraint::X].active = true;
    internal.dofConstraints[DoFConstraint::Y].active = true;
    internal.sourceObject = &firstRod;
    internal.targetObject = &secondRod;
    internal.sourceTransform.translate({150.f, 0});
    internal.targetTransform.translate({-250.f, 0});

    firstRod.rotate(sf::degrees(90));
    firstRod.setPosition({800, 128.f + 150.f});

    secondRod.setPosition({800 + 250.f, 128.f + 300.f});

    rail.init(solver);
    internal.init(solver);
    firstRod.init(solver);
    secondRod.init(solver);

    solver.draw = true;
    g_engine.registerTickable(&solver);
    g_engine.run();
    g_engine.shutdown();
}
