#include "PointMass.h"

void PointMass::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::CircleShape circle;
    circle.setRadius(32.f);
    circle.setPosition(getPosition() - sf::Vector2f{circle.getRadius(), circle.getRadius()});
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Blue);
    circle.setOutlineThickness(2.f);
    target.draw(circle);
}
