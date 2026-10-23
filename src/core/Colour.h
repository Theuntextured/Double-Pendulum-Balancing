#pragma once

#include <cstdint>
#include <ostream>
#include <string>
#include <string_view>

#include <SFML/Graphics/Color.hpp>
#include <imgui.h>

struct Colour {
    std::uint8_t r = 0;
    std::uint8_t g = 0;
    std::uint8_t b = 0;

    constexpr Colour() = default;

    constexpr Colour(std::uint8_t red,
                     std::uint8_t green,
                     std::uint8_t blue)
        : r(red), g(green), b(blue) {}

    // Implicit conversions. RGB colours are fully opaque.
    operator sf::Color() const {
        return sf::Color(r, g, b);
    }

    operator ImColor() const {
        return ImColor(int(r), int(g), int(b));
    }

    // Named conversions avoid ambiguity in overloaded ImGui calls.
    constexpr ImU32 toImU32() const {
        return IM_COL32(r, g, b, 255);
    }

    ImVec4 toImVec4() const {
        return ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
    }

    // ANSI true-colour escape sequence.
    std::string ansi(bool background = false) const {
        return std::string(background ? "\033[48;2;" : "\033[38;2;")
             + std::to_string(r) + ";"
             + std::to_string(g) + ";"
             + std::to_string(b) + "m";
    }
    static constexpr std::string_view ansiReset = "\033[0m";

    friend std::ostream& operator<<(std::ostream& out, Colour colour) {
        return out << colour.ansi();
    }

    inline static constexpr std::string_view Reset = "\033[0m";

    // Definitions follow the completed struct.
    static const Colour Black, Red, Green, Yellow;
    static const Colour Blue, Magenta, Cyan, White;
    static const Colour BrightBlack, BrightRed, BrightGreen, BrightYellow;
    static const Colour BrightBlue, BrightMagenta, BrightCyan, BrightWhite;
    static const Colour Orange, Grey, LightGrey;
};

// Fixed RGB palette; terminal theme colours may differ.
inline constexpr Colour Colour::Black         {  0,   0,   0};
inline constexpr Colour Colour::Red           {128,   0,   0};
inline constexpr Colour Colour::Green         {  0, 128,   0};
inline constexpr Colour Colour::Yellow        {128, 128,   0};
inline constexpr Colour Colour::Blue          {  0,   0, 128};
inline constexpr Colour Colour::Magenta       {128,   0, 128};
inline constexpr Colour Colour::Cyan          {  0, 128, 128};
inline constexpr Colour Colour::White         {192, 192, 192};

inline constexpr Colour Colour::BrightBlack   {128, 128, 128};
inline constexpr Colour Colour::BrightRed     {255,   0,   0};
inline constexpr Colour Colour::BrightGreen   {  0, 255,   0};
inline constexpr Colour Colour::BrightYellow  {255, 255,   0};
inline constexpr Colour Colour::BrightBlue    {  0,   0, 255};
inline constexpr Colour Colour::BrightMagenta {255,   0, 255};
inline constexpr Colour Colour::BrightCyan    {  0, 255, 255};
inline constexpr Colour Colour::BrightWhite   {255, 255, 255};

inline constexpr Colour Colour::Orange    {255, 165,   0};
inline constexpr Colour Colour::Grey      {128, 128, 128};
inline constexpr Colour Colour::LightGrey {211, 211, 211};