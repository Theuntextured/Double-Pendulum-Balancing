#pragma once
#include "PhysicsCore.h"
#include "core/Common.h"

struct DoFConstraint {
    bool active = false;

    union {
        struct { float lowerBound = 0, upperBound = 0; };
        struct { sf::Angle minAngle, maxAngle; };
    };
    
    float   linearDamping       = 0,
            constantDamping     = 0,
            returnConstantForce = 0,
            returnLinearForce   = 0;
    
    enum Type : size_t { X, Y, Theta, Num };

    NODISCARD bool isFixed() const { return lowerBound == upperBound && active; }
};


class Constraint : public sf::Drawable {
    friend class PhysicsSolver;
public:
    void init(class PhysicsSolver& solver);

    sf::Transform sourceTransform;
    sf::Transform targetTransform;

    PhysicsObject* sourceObject{nullptr};
    PhysicsObject* targetObject{nullptr};

    DoFConstraint dofConstraints[DoFConstraint::Num]{};

    //gets target transform relative to source
    sf::Transform getTargetRelativeTransform() const;

    NODISCARD bool isStatic() const { return !sourceObject || sourceObject->isStatic(); }

protected:
    enum Resource : size_t {
        Hinge,
        Slider,
        FixedBar,
        Spring,
        InternalHinge,
        HorizontalBar,
        VerticalBar,
        Hatch,
        Square,
        LimitedInternalHinge,
        Num,

        None = Num,

        Invalid
    };
    static sf::Texture s_spriteResources[Num];
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    mutable Resource m_cachedResource{Invalid};
public:
    static float s_iconSize;
    NODISCARD Resource getType() const;
};
