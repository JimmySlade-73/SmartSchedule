#include "ui/adapters/TableAdapter.hpp"

std::vector<sf::Text> tableAdapter(
    const std::vector<std::string>& header, const sf::Font& font, int size_font)
{
    std::vector<sf::Text> texts;
    for (const auto string : header) texts.push_back({font, string, (unsigned int)size_font});
    return texts;
}

std::vector<std::vector<sf::Text>> tableAdapter(
    const std::vector<TableEntry*>& entry, const sf::Font& font, int size_font)
{
    std::vector<std::vector<sf::Text>> texts;
    for (const auto& sub : entry) {
        texts.push_back(tableAdapter(sub->getFields(), font, size_font));
    }
    return texts;
}

std::vector<std::vector<sf::Text>> tableAdapter(
    const std::vector<std::vector<std::string>>& strings, const sf::Font& font, int size_font)
{
    std::vector<std::vector<sf::Text>> texts;
    for (const auto& string_row : strings) {
        texts.push_back(tableAdapter(string_row, font, size_font));
    }
    return texts;
}