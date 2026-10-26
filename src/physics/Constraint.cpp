#include "Constraint.h"

sf::Vector3f Constraint::getSourceVelocity() const {
    const auto vel = sourceObject->getVelocity() +
        sf::Vector2f(
            -sourceObject->getAngularVelocity() * localPosition.y,
            sourceObject->getAngularVelocity() * localPosition.x
            );
    return {vel.x, vel.y,  sourceObject->getAngularVelocity()};
}

sf::Vector3f Constraint::getTargetVelocity() const {
    const auto vel = targetObject->getVelocity() +
        sf::Vector2f(
            -targetObject->getAngularVelocity() * targetPosition.y,
            targetObject->getAngularVelocity() * targetPosition.x
            );
    return {vel.x, vel.y,  targetObject->getAngularVelocity()};
}

sf::Vector2f Constraint::getRightVector() const {
    return sf::Vector2f(1, 0).rotatedBy(sourceObject->getRotation()).rotatedBy(sourceRotation);
}

sf::Vector2f Constraint::getUpVector() const {
    return sf::Vector2f(0, 1).rotatedBy(sourceObject->getRotation()).rotatedBy(sourceRotation);
}

void Constraint::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    constexpr sf::Vector2u constraintOrigin{128, 40};

}

static const std::filesystem::path spriteSheetPath = "assets/textures/support-spritesheet.png";
sf::Texture Constraint::s_spriteResources[Constraint::Num] = {
    sf::Texture(spriteSheetPath, false, sf::IntRect({0  , 256 * 0}, {256, 256})),
    sf::Texture(spriteSheetPath, false, sf::IntRect({256, 256 * 0}, {256, 256})),
    sf::Texture(spriteSheetPath, false, sf::IntRect({0  , 256 * 1}, {256, 256})),
    sf::Texture(spriteSheetPath, false, sf::IntRect({256, 256 * 1}, {256, 256})),
    sf::Texture(spriteSheetPath, false, sf::IntRect({0  , 256 * 2}, {256, 256})),
    sf::Texture(spriteSheetPath, false, sf::IntRect({256, 256 * 2}, {256, 256})),
    sf::Texture(spriteSheetPath, false, sf::IntRect({0  , 256 * 3}, {256, 256})),
    sf::Texture(spriteSheetPath, false, sf::IntRect({256, 256 * 3}, {256, 256})),
    sf::Texture(spriteSheetPath, false, sf::IntRect({0  , 256 * 4}, {256, 256})),
    sf::Texture(spriteSheetPath, false, sf::IntRect({256, 256 * 4}, {256, 256})),
};
