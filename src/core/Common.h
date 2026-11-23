#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#define NODISCARD [[nodiscard]]
#define INDEX_NONE -1

#define STRINGIFY(X) STRINGIFY_INTERNAL(X)
#define STRINGIFY_INTERNAL(X) #X

#define MACRO_COMBINE(A,B) MACRO_COMBINE_INTERNAL(A,B)
#define MACRO_COMBINE_INTERNAL(A,B) A##B

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

inline sf::Vector2f transformDirection(const sf::Transform& t, const sf::Vector2f& v) {
    const auto matrix = t.getMatrix();
    return {matrix[0] * v.x + matrix[4] * v.y,
            matrix[1] * v.x + matrix[5] * v.y};
}

inline sf::Vector2f getTransformPosition(const sf::Transform& t) {
    const auto matrix = t.getMatrix();
    return {matrix[12], matrix[13]};
}

inline sf::Angle getTransformRotation(const sf::Transform& transform) {
    const float* m = transform.getMatrix();
    return sf::radians(std::atan2(m[1], m[0]));
}

inline sf::Vector2f getTransformScale(const sf::Transform& transform) {
    const float* m = transform.getMatrix();
    return {m[0], m[5]};
}

template <typename T, typename... ArgsType>
bool areAllEqual(const T& a, const T& b, ArgsType&&... args) {
    return (a == b) && areAllEqual(b, std::forward<ArgsType>(args)...);
}
template <typename T>
bool areAllEqual(const T& a, const T& b) {
    return a == b;
}

template <typename T>
T sign(const T& x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}
template <typename T>
T sign(const T& x, const T& tolerance) requires (std::is_floating_point_v<T>) {
    if (x > tolerance) return 1;
    if (x < -tolerance) return -1;
    return 0;
}