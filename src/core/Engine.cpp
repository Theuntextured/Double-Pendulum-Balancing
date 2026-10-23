#include "Engine.h"

#include "GraphicsEngine.h"

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

void Engine::run() {
    if (!m_isInitialized) {
        LOG(Engine, Error, "Engine not initialized!");
        return;
    }
    LOG(Engine, Info, "Engine running");
    sf::Clock clock;
    while (m_graphicsEngine->isOpen()) {
        const auto dt = clock.restart();
        m_graphicsEngine->preTick(dt);
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

Engine* const g_engine = new Engine();

