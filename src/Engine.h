#pragma once
#include "Common.h"
#include "Log.h"

class Engine {
    bool m_isInitialized{false};
    class GraphicsEngine* m_graphicsEngine{nullptr};
public:
    NODISCARD bool isInitialized() const { return m_isInitialized; }
    NODISCARD class GraphicsEngine* getGraphicsEngine() const { return m_graphicsEngine; }
    void init();
    void run();
    void shutdown();
};

DECLARE_LOG_GROUP(Engine)

extern Engine* const g_engine;
