#include "GraphicsEngine.h"

#include "Common.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include "physics/PhysicsSolver.h"

GraphicsEngine::GraphicsEngine()
    : sf::RenderWindow(
        sf::VideoMode({1600, 1200}),
        sf::String("Double Pendulum Balancing ") + APP_VERSION
        ) {
    (void)ImGui::SFML::Init(*this);
    (void)m_renderTarget.resize({1920,1080});
    m_renderTarget.clear(sf::Color::Transparent);
    (void)m_deferredRenderTarget.resize({1920, 1080});
    m_deferredRenderTarget.clear(sf::Color::Transparent);

}

void GraphicsEngine::preTick(sf::Time deltaTime) {
    clear();
    m_renderTarget.clear(sf::Color::Transparent);
    m_deferredRenderTarget.clear(sf::Color::Transparent);
    ImGui::SFML::Update(*this, deltaTime);
}

void GraphicsEngine::render() {
    ImGui::EndFrame();
    m_renderTarget.display();
    m_deferredRenderTarget.display();
    const sf::Sprite sprite(m_renderTarget.getTexture());
    const sf::Sprite deferredSprite(m_deferredRenderTarget.getTexture());
    sf::RenderStates states;
    states.transform.translate({0, static_cast<float>(m_renderTarget.getSize().y)});
    states.transform.scale({1,-1});
    draw(sprite, states);
    draw(deferredSprite, states);
    ImGui::SFML::Render(*this);
    display();
}

void GraphicsEngine::render(const sf::Drawable &drawable, bool deferred) {
    (deferred ? m_deferredRenderTarget : m_renderTarget).draw(drawable);
}

GraphicsEngine::~GraphicsEngine() {
    ImGui::SFML::Shutdown(*this);
}
