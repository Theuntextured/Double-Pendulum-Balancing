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
    m_clock.restart();
    while (m_graphicsEngine->isOpen()) {
        const auto dt = clock.restart();
        m_graphicsEngine->preTick(dt);
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

