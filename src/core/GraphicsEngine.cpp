#include "GraphicsEngine.h"

#include "Common.h"
#include "Engine.h"
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
}

void GraphicsEngine::preTick(sf::Time deltaTime) {
    processEvents();
    clear();
    m_renderTarget.clear(sf::Color::Transparent);
    ImGui::SFML::Update(*this, deltaTime);
}

void GraphicsEngine::render() {
    ImGui::EndFrame();
    m_renderTarget.display();
    const sf::Sprite sprite(m_renderTarget.getTexture());
    sf::RenderStates states;
    states.transform.translate({0, static_cast<float>(m_renderTarget.getSize().y)});
    states.transform.scale({1,-1});
    draw(sprite, states);
    ImGui::SFML::Render(*this);
    display();
}

void GraphicsEngine::processEvents() {
    while (const auto event = pollEvent()) {
        ImGui::SFML::ProcessEvent(*this, *event);


        if ( event->is<sf::Event::Closed>() ) {
            close();
            return;
        }

        if (m_interactiveSolver) {
            if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
                m_grabbedConstraint = m_interactiveSolver->getConstraintAt(static_cast<sf::Vector2f>(event->getIf<sf::Event::MouseButtonPressed>()->position));
            else if (event->is<sf::Event::MouseButtonReleased>() && event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left)
                m_grabbedConstraint = nullptr;
        }
    }

    
}

void GraphicsEngine::render(const sf::Drawable &drawable) {
    m_renderTarget.draw(drawable);
}

GraphicsEngine::~GraphicsEngine() {
    ImGui::SFML::Shutdown(*this);
}
