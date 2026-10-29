#include "Constraint.h"

#include "PhysicsSolver.h"
#include "core/Engine.h"


void Constraint::init(class PhysicsSolver &solver) {
    solver.m_constraints.emplace(this);
}

void Constraint::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    const Resource type = getType();

    if (type == None)
        return;

    if (type >= Num) {
        LOG(Physics, Error, "Invalid constraint resource type.");
        return;
    }

    constexpr float cellSize = 256.f;
    constexpr sf::Vector2f junction{128.f, 40.f};

    const float size = s_iconSize;
    if (size <= 0.f)
        return;

    const auto& x = dofConstraints[DoFConstraint::X];
    const auto& y = dofConstraints[DoFConstraint::Y];
    const auto& theta = dofConstraints[DoFConstraint::Theta];


    if (sourceObject)
        states.transform.combine(sourceObject->getTransform());

    states.transform *= sourceTransform;

    // originPx is measured in the original 256×256 texture.
    const auto drawQuad = [&](
        Resource resource,
        sf::RenderStates drawStates,
        sf::Vector2f originPx,
        sf::Vector2f dimensions,
        sf::Vector2f uvMax
    ) {
        const sf::Vector2f offset{
            originPx.x / cellSize * dimensions.x,
            originPx.y / cellSize * dimensions.y
        };

        const std::array vertices{
            sf::Vertex{
                .position = sf::Vector2f{0.f, 0.f} - offset,
                .texCoords = {0.f, 0.f}
            },
            sf::Vertex{
                .position = sf::Vector2f{0.f, dimensions.y} - offset,
                .texCoords = {0.f, uvMax.y}
            },
            sf::Vertex{
                .position = sf::Vector2f{dimensions.x, 0.f} - offset,
                .texCoords = {uvMax.x, 0.f}
            },
            sf::Vertex{
                .position = dimensions - offset,
                .texCoords = uvMax
            }
        };

        drawStates.texture = &s_spriteResources[resource];
        drawStates.coordinateType = sf::CoordinateType::Normalized;
        drawStates.transform.scale({1.f, -1.f});

        target.draw(
            vertices.data(),
            vertices.size(),
            sf::PrimitiveType::TriangleStrip,
            drawStates
        );
    };

    const auto drawIcon = [&](
        Resource resource,
        const sf::RenderStates &drawStates,
        sf::Vector2f originPx
    ) {
        drawQuad(resource, drawStates, originPx,
                 {size, size}, {1.f, 1.f});
    };

    switch (type) {
        case Slider: {
            const bool verticalTravel = x.isFixed();

            auto floorStates = states;
            if (verticalTravel)
                floorStates.transform.rotate(sf::degrees(90.f));

            const auto& travel = verticalTravel ? y : x;

            if (travel.active &&
                std::isfinite(travel.lowerBound) &&
                std::isfinite(travel.upperBound) &&
                travel.upperBound > travel.lowerBound)
            {
                // A +90° rotation maps the icon's +X onto local +Y.
                const float lower = travel.lowerBound;
                const float upper = travel.upperBound;

                // Roller bottom: y=223; junction: y=40.
                constexpr float floorOffset = (223.f - 40.f) / cellSize;

                floorStates.transform.translate({
                    lower,
                    floorOffset * size
                });
                // Floor top edge touches the rollers
                drawQuad(
                    Hatch,
                    floorStates,
                    {0.f, -167},
                    {upper - lower, size},
                    {(upper - lower) / size, 1.f}
                );
            }

            auto rollerStates = states;
            rollerStates.transform.translate(
                getTargetRelativeTransform().transformPoint({0.f, 0.f})
            );
            if (verticalTravel)
                rollerStates.transform.rotate(sf::degrees(90.f));
            drawIcon(Slider, rollerStates, junction);
            break;
        }

        case LimitedInternalHinge: {
            auto lowerStates = states;
            lowerStates.transform.rotate(theta.minAngle);
            drawIcon(LimitedInternalHinge, lowerStates, junction);

            if (theta.upperBound != theta.lowerBound) {
                auto upperStates = states;
                upperStates.transform.rotate(theta.maxAngle);
                drawIcon(LimitedInternalHinge, upperStates, junction);
            }
            break;
        }

        case Spring: {
            auto springStates = states;

            if (y.isFixed())
                springStates.transform.rotate(sf::degrees(90.f));

            drawIcon(Spring, springStates, junction);
            break;
        }

        case Hatch:
            drawIcon(Hatch, states, {128.f, 202.f});
            break;

        case HorizontalBar:
            drawIcon(HorizontalBar, states, {128.f, 223.f});
            break;

        case VerticalBar:
            drawIcon(VerticalBar, states, {128.f, 128.f});
            break;

        default:
            drawIcon(type, states, junction);
            break;
    }
}

Constraint::Resource Constraint::getType() const {
    if (m_cachedResource != Invalid) return m_cachedResource;

    const auto& xConstraint     = dofConstraints[   DoFConstraint::X    ];
    const auto& yConstraint     = dofConstraints[   DoFConstraint::Y    ];
    const auto& thetaConstraint = dofConstraints[   DoFConstraint::Theta];

    if (xConstraint.isFixed()) {
        // Hinge, RSlider, Spring, InternalHinge, Hatch, None, LimitedInternalHinge
        if (yConstraint.isFixed()) {
            // Hinge, InternalHinge, Hatch, None, LimitedInternalHinge
            if (thetaConstraint.isFixed()) {
                //Hatch, None
                if (!sourceObject || sourceObject->isStatic()) return m_cachedResource = Hatch;
                return m_cachedResource = None;
            }
            // Hinge, InternalHinge, LimitedInternalHinge
            if (thetaConstraint.active) return m_cachedResource =  LimitedInternalHinge;
            if (!sourceObject || sourceObject->isStatic()) return m_cachedResource = Hinge;
            return m_cachedResource = InternalHinge;
        }
        // RSlider, Spring
        if (yConstraint.returnConstantForce > 0 || yConstraint.returnLinearForce > 0) return m_cachedResource = Spring;
        return m_cachedResource = Slider;
    }
    // Slider, RSpring
    if (xConstraint.returnConstantForce > 0 || xConstraint.returnLinearForce > 0) return m_cachedResource = Spring;
    if (!thetaConstraint.active && yConstraint.isFixed())
        return m_cachedResource = Slider;
    return m_cachedResource = Square;
}

static sf::Texture makeSpriteSheetTexture(int index) {
    static const std::filesystem::path spriteSheetPath = "assets/textures/support-spritesheet.png";
    sf::Texture texture(spriteSheetPath, false, sf::IntRect({(index % 2) * 256 , 256 * (index / 2)}, {256, 256}));
    texture.setSmooth(true);
    texture.setRepeated(true);
    return texture;
}


sf::Transform Constraint::getTargetRelativeTransform() const {
    const sf::Transform sourceWorld = sourceObject
        ? sourceObject->getTransform() * sourceTransform
        : sourceTransform;

    const sf::Transform targetWorld = targetObject
        ? targetObject->getTransform() * targetTransform
        : targetTransform;

    return sourceWorld.getInverse() * targetWorld;
}

sf::Transform Constraint::getSourceWorldTransform() const {
    return sourceObject
        ? sourceObject->getTransform() * sourceTransform
        : sourceTransform;
}

sf::Transform Constraint::getTargetWorldTransform() const {
    return targetObject->getTransform() * targetTransform;
}

sf::Texture Constraint::s_spriteResources[Num] = {
    makeSpriteSheetTexture(0),
    makeSpriteSheetTexture(1),
    makeSpriteSheetTexture(2),
    makeSpriteSheetTexture(3),
    makeSpriteSheetTexture(4),
    makeSpriteSheetTexture(5),
    makeSpriteSheetTexture(6),
    makeSpriteSheetTexture(7),
    makeSpriteSheetTexture(8),
    makeSpriteSheetTexture(9),
};

float Constraint::s_iconSize = 128.f;
