#include "GraphicsEngine.h"

#include "Common.h"
#include "imgui-SFML.h"
#include "imgui.h"

GraphicsEngine::GraphicsEngine()
    : sf::RenderWindow(
        sf::VideoMode({1600, 1200}),
        sf::String("Double Pendulum Balancing ") + APP_VERSION
        ) {
    (void)ImGui::SFML::Init(*this);
    (void)m_renderTarget.resize({1920,1080});
    m_renderTarget.clear(sf::Color::Transparent);
}

void GraphicsEngine::preTick(sf::Time deltaTime) {
    while (const auto event = pollEvent()) {
        if ( event->is<sf::Event::Closed>() )
            close();
        ImGui::SFML::ProcessEvent(*this, *event);
    }
    clear();
    m_renderTarget.clear(sf::Color::Transparent);
    ImGui::SFML::Update(*this, deltaTime);
}

void GraphicsEngine::render() {
    ImGui::EndFrame();
    m_renderTarget.display();
    const sf::Sprite sprite(m_renderTarget.getTexture());
    draw(sprite);
    ImGui::SFML::Render(*this);
    display();
}

void GraphicsEngine::render(const sf::Drawable &drawable) {
    m_renderTarget.draw(drawable);
}

GraphicsEngine::~GraphicsEngine() {
    ImGui::SFML::Shutdown(*this);
}
