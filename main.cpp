#include "physics/Beam.h"
#include "physics/PhysicsSolver.h"
#include "physics/PointMass.h"
#include "src/core/Engine.h"

int main()
{
    g_engine.init();

    PhysicsSolver solver;

    constexpr float railY = 1024.f;

    Beam firstRod(300.f, 1.f);
    Beam secondRod(500.f, 1.5f);
    Constraint rail;
    auto& railXConstraint = rail.dofConstraints[DoFConstraint::X];
    railXConstraint.active = true;
    railXConstraint.lowerBound = -600;
    railXConstraint.upperBound = 600;
    rail.dofConstraints[DoFConstraint::Y].active = true;
    rail.sourceTransform.translate({800, railY});
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
    firstRod.setPosition({800, railY + 150.f});

    secondRod.setPosition({800 + 250.f, railY + 300.f});

    PointMass pointMass;

    pointMass.setMass(5.f);
    pointMass.setPosition({800 + 500.f, railY + 300.f});

    Constraint rodToPointMass;
    rodToPointMass.sourceTransform.translate({250.f, 0});
    rodToPointMass.sourceObject = &secondRod;
    rodToPointMass.targetObject = &pointMass;
    for (size_t i = 0; i < 2; i++) rodToPointMass.dofConstraints[i].active = true;


    rail.init(solver);
    internal.init(solver);
    firstRod.init(solver);
    secondRod.init(solver);
    rodToPointMass.init(solver);
    pointMass.init(solver);

    solver.draw = true;
    g_engine.registerTickable(&solver);
    g_engine.run();
    g_engine.shutdown();
}
