#pragma once
#include <unordered_set>

#include "Common.h"
#include "Log.h"

class Engine {
    bool m_isInitialized{false};
    class GraphicsEngine* m_graphicsEngine{nullptr};

    std::unordered_set<Tickable*> tickables_;
    bool isTicking{false};
    sf::Clock m_clock;
public:
    NODISCARD bool isInitialized() const { return m_isInitialized; }
    NODISCARD class GraphicsEngine* getGraphicsEngine() const { return m_graphicsEngine; }
    void init();
    void run();
    void shutdown();

    sf::Time getTotalTime() const { return m_clock.getElapsedTime(); }

    //do not use while ticking
    void registerTickable(Tickable* tickable);

    //do not use while ticking
    void unregisterTickable(Tickable* tickable);
};

DECLARE_LOG_GROUP(Engine)

extern Engine g_engine;
