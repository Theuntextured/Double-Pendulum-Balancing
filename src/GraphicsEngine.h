#pragma once
#include "SFML/Graphics.hpp"

class GraphicsEngine : sf::RenderWindow {
    using Super = sf::RenderWindow;
    friend class Engine;
    GraphicsEngine();
    void preTick(sf::Time deltaTime);
    void render();
    ~GraphicsEngine() override;
public:
};
