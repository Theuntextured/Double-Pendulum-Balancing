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
}

void GraphicsEngine::preTick(sf::Time deltaTime) {
    while (const auto event = pollEvent()) {
        if ( event->is<sf::Event::Closed>() )
            close();
        ImGui::SFML::ProcessEvent(*this, *event);
    }
    clear();
    ImGui::SFML::Update(*this, deltaTime);
}

void GraphicsEngine::render() {
    ImGui::EndFrame();
    ImGui::SFML::Render(*this);
    display();
}

GraphicsEngine::~GraphicsEngine() {
    ImGui::SFML::Shutdown(*this);
}
