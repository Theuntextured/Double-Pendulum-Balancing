#include "Beam.h"

Beam::Beam(float length, float mass) {
    m_length = length;
    m_mass = mass;
}

float Beam::getMomentOfInertia() const {
    return getMass() * m_length * m_length / 12;
}

sf::Vector2f Beam::getLocalCenterOfMass() const {
    return {m_length * 0.5f, 0};
}

void Beam::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    constexpr float thickness = 3.0f;
    constexpr float halfThickness = thickness * 0.5f;

    const float halfLength = m_length * 0.5f;

    std::array<sf::Vertex, 4> line;
    line[0].position = sf::Vector2f(halfLength, -halfThickness);
    line[1].position = sf::Vector2f(halfLength, halfThickness);
    line[2].position = sf::Vector2f(-halfLength, -halfThickness);
    line[3].position = sf::Vector2f(-halfLength, halfThickness);

    states.transform *= getTransform();
    target.draw(line.data(), 4, sf::PrimitiveType::TriangleStrip, states);
}

