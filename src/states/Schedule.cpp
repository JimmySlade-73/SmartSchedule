#include <states/Schedule.hpp>
#include <iostream>
#include <ui/adapters/TableAdapter.hpp>
#include "states/SelectSchedule.hpp"

Schedule::Schedule(App& app_instance) : State(app_instance)
{
    if (!font.openFromFile("../resources/fonts/arial.ttf")) {
        std::cout << "error al cargar la fuente" << std::endl;
    }
    float size_row = app.getSizeByPercent(0.f, 1.f/16).y;
    schedule.setSize(app.getSizeByPercent(1.f, 7.f/8));
    schedule.setSizeRow(size_row);
    schedule.setProportions({1, 1, 1, 1, 1, 1, 1, 1});
    schedule.setHeaders(tableAdapter(Subject::getHeadersSchedule(), font, size_font));
    schedule.setTexts(tableAdapter(app.getEngine().getSchedule(7, 22), font, size_font));
    schedule.setBodyFunc(true, true);

    buttons["return"] = std::make_unique<Button>();
    buttons["return"]->setText({font, "return", (unsigned int)size_font});
    buttons["return"]->setPosition(app.getSizeByPercent(7.f/8, 7.f/8));
    buttons["return"]->setSize(app.getSizeByPercent(1.f/8, 1.f/8));
    buttons["return"]->setFunc( [this](){ buttonReturn(); } );
}

void Schedule::handleInput(std::optional<sf::Event> event)
{
    schedule.handleEvent(*event);
    for (auto& [name, btn] : buttons) {
        btn->handleEvent(*event);
    }
}

void Schedule::update(float dt)
{
    sf::RenderWindow& window = app.getWindow();
    auto mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    schedule.update(dt, mouse_pos);
    for (auto& [name, btn] : buttons) {
        btn->update(dt, mouse_pos);
    }
}

void Schedule::draw(sf::RenderWindow& window)
{
    window.draw(schedule);
    for (auto& [name, btn] : buttons) {
        window.draw(*btn);
    }
}

void Schedule::buttonReturn()
{
    app.changeState(std::make_unique<SelectSchedule>(app));
}