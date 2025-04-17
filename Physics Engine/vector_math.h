#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>

inline float magnitude(const sf::Vector2f& v)		           { return std::sqrt(v.x * v.x + v.y * v.y); };
inline float squared_magnitude(const sf::Vector2f& v)          { return v.x * v.x + v.y * v.y; };
inline float dot(const sf::Vector2f& v, const sf::Vector2f& w) { return v.x * w.x + v.y * w.y; };
inline sf::Angle getAngle(const sf::Vector2f& vec)             { return sf::radians(std::atan2(vec.y, vec.x)); }