#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "data/Subject.hpp"

std::vector<sf::Text> tableAdapter(
    const std::vector<std::string>& header, const sf::Font& font, int size_font);

std::vector<std::vector<sf::Text>> tableAdapter(
    const std::vector<TableEntry*>& entry, const sf::Font& font, int size_font);

std::vector<std::vector<sf::Text>> tableAdapter(
    const std::vector<std::vector<std::string>>& strings, const sf::Font& font, int size_font);