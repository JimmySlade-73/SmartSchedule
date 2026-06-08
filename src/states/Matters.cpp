#include <states/Matters.hpp>
#include <iostream>
#include <ui/adapters/TableAdapter.hpp>
#include "states/Matters.hpp"
#include "states/SelectSchedule.hpp"

Matters::Matters(App& app_instance) : State(app_instance)
{
    if (!font.openFromFile("../resources/fonts/arial.ttf")) {
        std::cout << "error al cargar la fuente" << std::endl;
    }
    float size_row = app.getSizeByPercent(0.f, 1.f/16).y;
    matters.setSize(app.getSizeByPercent(1.f/2, 1.f));
    matters.setSizeRow(size_row);
    matters.setProportions({1});
    matters.setHeaders(tableAdapter({"Matters"}, font, size_font));
    std::vector<std::vector<std::string>> texts;
    for (auto& a : app.getEngine().getCompletedMatters()) {
        texts.push_back({a});
    }
    matters.setTexts(tableAdapter(texts, font, size_font));
    matters.setBodyFunc(true, false);

    buttons["return"] = std::make_unique<Button>();
    buttons["return"]->setText({font, "return", (unsigned int)size_font});
    buttons["return"]->setPosition(app.getSizeByPercent(7.f/8, 7.f/8));
    buttons["return"]->setSize(app.getSizeByPercent(1.f/8, 1.f/8));
    buttons["return"]->setFunc( [this](){ buttonReturn(); } );

    buttons["quit"] = std::make_unique<Button>();
    buttons["quit"]->setText({font, "quit", (unsigned int)size_font});
    buttons["quit"]->setPosition(app.getSizeByPercent(6.f/8, 7.f/8));
    buttons["quit"]->setSize(app.getSizeByPercent(1.f/8, 1.f/8));
    buttons["quit"]->setFunc( [this](){ buttonQuit(); } );
}

void Matters::handleInput(std::optional<sf::Event> event)
{
    for (auto& [name, btn] : buttons) {
        btn->handleEvent(*event);
    }
    matters.handleEvent(*event);
}

void Matters::update(float dt)
{
    sf::RenderWindow& window = app.getWindow();
    auto mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    for (auto& [name, btn] : buttons) {
        btn->update(dt, mouse_pos);
    }
    matters.update(dt, mouse_pos);
}

void Matters::draw(sf::RenderWindow& window)
{
    window.draw(matters);
    for (auto& [name, btn] : buttons) {
        window.draw(*btn);
    }
}

void Matters::buttonReturn()
{
    app.changeState(std::make_unique<SelectSchedule>(app));
}

void Matters::buttonQuit()
{
    app.getEngine().quitMatterByIndex(matters.getSelected().row);
    std::vector<std::vector<std::string>> texts;
    for (auto& a : app.getEngine().getCompletedMatters()) {
        texts.push_back({a});
    }
    matters.setTexts(tableAdapter(texts, font, size_font));
    matters.setBodyFunc(true, false);
}