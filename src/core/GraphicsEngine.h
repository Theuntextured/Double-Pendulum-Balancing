#pragma once
#include "SFML/Graphics.hpp"

class GraphicsEngine : sf::RenderWindow {
    friend class Engine;

    GraphicsEngine();
    ~GraphicsEngine() override;

    void preTick(sf::Time deltaTime);
    void render();

    sf::RenderTexture m_renderTarget;
    sf::RenderTexture m_deferredRenderTarget;
public:
    void render(const sf::Drawable& drawable, bool deferred = false);
};
