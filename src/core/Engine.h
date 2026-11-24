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

    class PhysicsSolver* m_interactiveSolver{nullptr};
    class Constraint* m_grabbedConstraint{nullptr};
    float m_pullStrength{0.5f};

    void processEvents();

    sf::Vector2f m_grabPoint;
    sf::Vector2f m_grabTarget;
    float m_grabStrength{0};
public:
    sf::Vector2f getGrabPoint() const { return m_grabPoint; }
    sf::Vector2f getGrabTarget() const { return m_grabTarget; }
    float getGrabStrength() const { return m_grabStrength; }

    void setInteractiveSolver(class PhysicsSolver* solver);
    void setPullStrength(float strength) { m_pullStrength = fmax(strength, 0); }
    float getPullStrength() const { return m_pullStrength; }
    class PhysicsSolver* getInteractiveSolver() const { return m_interactiveSolver; }
    class Constraint* getGrabbedConstraint() const { return m_grabbedConstraint; }

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

    float grabDistance = 100.f;
};

DECLARE_LOG_GROUP(Engine)

extern Engine g_engine;
