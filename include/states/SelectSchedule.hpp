#pragma once
#include <iostream>
#include "State.hpp"
#include "ui/Button.hpp"
#include "ui/TableWidget.hpp"
#include "core/App.hpp"
#include <unordered_map>
#include <memory>

class SelectSchedule : public State {
private:
    int size_font = 18;
    sf::Font font;
    std::vector<Button> button_pool;
    std::unordered_map<std::string, std::unique_ptr<Button>> buttons;
    std::unordered_map<std::string, std::unique_ptr<TableWidget>> tables;

public:
    SelectSchedule(App& app_instance);
    void handleInput(std::optional<sf::Event> event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void buttonSelect();
    void buttonUnselect();
    void buttonCompletedMatter();
    void buttonSchedule();
    void buttonMatters();
};