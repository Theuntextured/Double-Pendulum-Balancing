#pragma once
#include "SFML/Graphics.hpp"

class GraphicsEngine : sf::RenderWindow {
    friend class Engine;

    GraphicsEngine();
    ~GraphicsEngine() override;

    void preTick(sf::Time deltaTime);
    void render();

    void processEvents();

    sf::RenderTexture m_renderTarget;

    class PhysicsSolver* m_interactiveSolver{nullptr};
    class Constraint* m_grabbedConstraint{nullptr};
public:
    void render(const sf::Drawable& drawable);
};
