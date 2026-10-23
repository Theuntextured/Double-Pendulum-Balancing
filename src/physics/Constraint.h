#pragma once
#include "PhysicsCore.h"
#include "core/Common.h"

struct DegreeOfFreedomConstraint {
    bool active = false;
    
    float   lowerBound          = 0, 
            upperBound          = 0, 
            linearDamping       = 0,
            constantDamping     = 0,
            returnConstantForce = 0,
            returnLinearForce   = 0;
    
    enum Type : size_t { X, Y, Theta, Num };
};

struct Constraint {
    sf::Vector2f localPosition;
    sf::Vector2f targetPosition;
    PhysicsObject* fixedOnObject{nullptr};
    PhysicsObject* targetObject{nullptr};
    DegreeOfFreedomConstraint dofConstraints[DegreeOfFreedomConstraint::Num]{};
};