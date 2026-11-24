#include "Engine.h"

#include "GraphicsEngine.h"
#include "imgui-SFML.h"
#include "physics/PhysicsSolver.h"

DEFINE_LOG_GROUP(Engine, Info);

void Engine::init() {
    if (m_isInitialized) {
        LOG(Engine, Error, "Engine already initialized!");
        return;
    }
    LOG(Engine, Info, "Engine initializing");

    m_graphicsEngine = new GraphicsEngine();

    LOG(Engine, Info, "Engine initialized");
    m_isInitialized = true;
}


void Engine::setInteractiveSolver(PhysicsSolver* solver) {
    if (m_interactiveSolver)
        m_interactiveSolver->clearDragTarget();
    m_interactiveSolver = solver;
    m_grabbedConstraint = nullptr;
}

void Engine::processEvents() {
    if (!m_graphicsEngine) return;
    const auto toWorld = [&](sf::Vector2i pixel) {
        auto world = m_graphicsEngine->mapPixelToCoords(pixel);
        world.y = static_cast<float>(m_graphicsEngine->m_renderTarget.getSize().y) - world.y;
        return world;
    };
    m_grabTarget = toWorld(sf::Mouse::getPosition(*m_graphicsEngine));

    Constraint* hoveringConstraint = nullptr;
    if (m_interactiveSolver && !m_grabbedConstraint)
        hoveringConstraint = m_interactiveSolver->getConstraintAt(m_grabTarget, grabDistance);

    if (hoveringConstraint) {
        sf::CircleShape circle(grabDistance);
        circle.setFillColor(sf::Color(25, 200, 35, 200));
        circle.setPosition(getTransformPosition(hoveringConstraint->getTargetWorldTransform()) - sf::Vector2f{circle.getRadius(), circle.getRadius()});
        m_graphicsEngine->render(circle, true);
    }

    while (const auto event = m_graphicsEngine->pollEvent()) {
        ImGui::SFML::ProcessEvent(*m_graphicsEngine, *event);


        if ( event->is<sf::Event::Closed>() ) {
            getGraphicsEngine()->close();
            return;
        }

        if (m_interactiveSolver) {
            if (const auto* pressed = event->getIf<sf::Event::MouseButtonPressed>();
                pressed && pressed->button == sf::Mouse::Button::Left) {
                m_grabTarget = toWorld(pressed->position);
                m_grabbedConstraint = m_interactiveSolver->getConstraintAt(m_grabTarget, grabDistance);
            } else if ((event->is<sf::Event::MouseButtonReleased>() && event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left)
                || event->is<sf::Event::FocusLost>()) {
                m_grabbedConstraint = nullptr;
            }
        }
    }

    if (m_interactiveSolver)
        m_interactiveSolver->setDragTarget(m_grabbedConstraint, m_grabTarget, m_pullStrength);

    if (m_interactiveSolver && m_grabbedConstraint) {
        m_grabPoint = getTransformPosition(m_grabbedConstraint->getTargetWorldTransform());

        const sf::Vector2f direction = m_grabTarget - m_grabPoint;
        const float length = direction.length();
        const float angle = std::atan2(direction.y, direction.x);

        sf::RectangleShape line(sf::Vector2f(length, 3.f));
        line.setPosition(m_grabPoint);
        line.setRotation(sf::radians(angle));
        line.setFillColor(sf::Color::Red);
        m_graphicsEngine->render(line, true);
    }
}

void Engine::run() {
    if (!m_isInitialized) {
        LOG(Engine, Error, "Engine not initialized!");
        return;
    }
    LOG(Engine, Info, "Engine running");
    sf::Clock clock;
    m_clock.restart();
    while (m_graphicsEngine->isOpen()) {
        const auto dt = clock.restart();
        m_graphicsEngine->preTick(dt);
        processEvents();
        isTicking = true;
        for (const auto tickable : tickables_)
            tickable->tick(dt.asSeconds());
        isTicking = false;
        m_graphicsEngine->render();
    }
}

void Engine::shutdown() {
    LOG(Engine, Info, "Engine shutting down");
    if (!m_isInitialized) {
        LOG(Engine, Error, "Engine not initialized!");
        return;
    }
    delete m_graphicsEngine;
    m_isInitialized = false;
    LOG(Engine, Info, "Engine shutdown");
}

void Engine::registerTickable(Tickable *tickable) {
    if (!tickable) return;
    if (isTicking) {
        LOG(Engine, Error, "Tickables cannot be registered after the engine has started ticking!");
        return;
    }
    tickables_.insert(tickable);
}

void Engine::unregisterTickable(Tickable *tickable) {
    if (!tickable) return;
    if (!isTicking) {
        LOG(Engine, Fatal, "Tickables cannot be unregistered before the engine has started ticking!");
        exit(-1);
        return;
    }
    tickables_.erase(tickable);
}

Engine g_engine;

