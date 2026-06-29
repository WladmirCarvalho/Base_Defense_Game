#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

inline float getDistance(sf::Vector2f p1, sf::Vector2f p2) {
    return std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));
}

inline sf::Vector2f normalizeVector(sf::Vector2f vec) {
    float length = std::sqrt(vec.x * vec.x + vec.y * vec.y);
    if (length != 0) return sf::Vector2f(vec.x / length, vec.y / length);
    return vec;
}