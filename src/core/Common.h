#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#define NODISCARD [[nodiscard]]
#define INDEX_NONE -1

#define STRINGIFY(X) STRINGIFY_INTERNAL(X)
#define STRINGIFY_INTERNAL(X) #X

#ifndef APP_VERSION
    #define APP_VERSION "v0.0.0-dev"
#endif

#define BIG_NUMBER 1e30
#define VERY_BIG_NUMBER 1e100

inline constexpr float PI = 3.14159265358979323846f;
inline constexpr float TWO_PI = 2.f * PI;

inline constexpr float toDegrees(float radians) { return radians * 180.f / PI; }
inline constexpr float toRadians(float degrees) { return degrees * PI / 180.f; }

class CountIterator {
public:
    CountIterator(size_t in_count) : count(in_count) {}
    size_t count;

    size_t operator*() const { return count; }
    CountIterator& operator++() { ++count; return *this; }
    bool operator!=(const CountIterator& other) const { return count != other.count; }
    bool operator==(const CountIterator &other) const { return count == other.count; }

    CountIterator begin() const { return CountIterator(0); }
    CountIterator end() const { return CountIterator(count); }
};

class Tickable {
public:
    virtual ~Tickable() = default;

    virtual void tick(float deltaTime) {};
};