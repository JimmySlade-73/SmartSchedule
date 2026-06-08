#include "states/SelectSchedule.hpp"
#include "states/Schedule.hpp"
#include "states/Matters.hpp"
#include "ui/adapters/TableAdapter.hpp"
#include <iostream>
#include <filesystem>

SelectSchedule::SelectSchedule(App& app_instance) : State(app_instance)
{
    if (!font.openFromFile("../resources/fonts/arial.ttf")) {
        std::cout << "error al cargar la fuente" << std::endl;
    }
    buttons["Prueba"] = std::make_unique<Button>();
    buttons["Prueba"]->setText({font, "prueba", (unsigned int)size_font});
    buttons["Prueba"]->setPosition(app.getSizeByPercent(7.f/8, 7.f/8));
    buttons["Prueba"]->setSize(app.getSizeByPercent(1.f/8, 1.f/8));
    buttons["Prueba"]->setFunc([](){ sf::Mouse::setPosition({0,0}); });

    buttons["select"] = std::make_unique<Button>();
    buttons["select"]->setText({font, "select", (unsigned int)size_font});
    buttons["select"]->setPosition(app.getSizeByPercent(4.f/8, 7.f/8));
    buttons["select"]->setSize(app.getSizeByPercent(1.f/8, 1.f/8));
    buttons["select"]->setFunc([this](){ buttonSelect(); });
    
    buttons["unselect"] = std::make_unique<Button>();
    buttons["unselect"]->setText({font, "unselect", (unsigned int)size_font});
    buttons["unselect"]->setPosition(app.getSizeByPercent(5.f/8, 7.f/8));
    buttons["unselect"]->setSize(app.getSizeByPercent(1.f/8, 1.f/8));
    buttons["unselect"]->setFunc([this](){ buttonUnselect(); });
    
    buttons["completed"] = std::make_unique<Button>();
    buttons["completed"]->setText({font, "completed", (unsigned int)size_font});
    buttons["completed"]->setPosition(app.getSizeByPercent(6.f/8, 7.f/8));
    buttons["completed"]->setSize(app.getSizeByPercent(1.f/8, 1.f/8));
    buttons["completed"]->setFunc([this](){ buttonCompletedMatter(); });
    
    buttons["schedule"] = std::make_unique<Button>();
    buttons["schedule"]->setText({font, "schedule", (unsigned int)size_font});
    buttons["schedule"]->setPosition(app.getSizeByPercent(4.f/8, 6.f/8));
    buttons["schedule"]->setSize(app.getSizeByPercent(1.f/8, 1.f/8));
    buttons["schedule"]->setFunc([this](){ buttonSchedule(); });
    
    buttons["matters"] = std::make_unique<Button>();
    buttons["matters"]->setText({font, "matters", (unsigned int)size_font});
    buttons["matters"]->setPosition(app.getSizeByPercent(5.f/8, 6.f/8));
    buttons["matters"]->setSize(app.getSizeByPercent(1.f/8, 1.f/8));
    buttons["matters"]->setFunc([this](){ buttonMatters(); });
    
    buttons["save"] = std::make_unique<Button>();
    buttons["save"]->setText({font, "save", (unsigned int)size_font});
    buttons["save"]->setPosition(app.getSizeByPercent(6.f/8, 6.f/8));
    buttons["save"]->setSize(app.getSizeByPercent(1.f/8, 1.f/8));
    buttons["save"]->setFunc([this](){ buttonSave(); });
    
    buttons["load"] = std::make_unique<Button>();
    buttons["load"]->setText({font, "load", (unsigned int)size_font});
    buttons["load"]->setPosition(app.getSizeByPercent(7.f/8, 6.f/8));
    buttons["load"]->setSize(app.getSizeByPercent(1.f/8, 1.f/8));
    buttons["load"]->setFunc([this](){ buttonLoad(); });
    

    std::vector<float> proportions = {1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1};
    float size_row = app.getSizeByPercent(0.f, 1.f/16).y;
    tables["filtred"] = std::make_unique<TableWidget>();
    tables["filtred"]->setSize(app.getSizeByPercent(1.f/2, 1.f));
    tables["filtred"]->setSizeRow(size_row);
    tables["filtred"]->setHeaders(tableAdapter(Subject::getHeaders(), font, size_font));
    tables["filtred"]->setProportions(proportions);
    tables["filtred"]->setTexts(tableAdapter(app.getEngine().getAll(), font, size_font));
    tables["filtred"]->setBodyFunc(true, true);
    tables["selected"] = std::make_unique<TableWidget>();
    tables["selected"]->setPosition(app.getSizeByPercent(1.f/2, 0.f));
    tables["selected"]->setSize(app.getSizeByPercent(1.f/2, 1.f/2));
    tables["selected"]->setSizeRow(size_row);
    tables["selected"]->setHeaders(tableAdapter(Subject::getHeaders(), font, size_font));
    tables["selected"]->setProportions(proportions);
    tables["selected"]->filtredByRows(app.getEngine().getSelected());
    tables["selected"]->setTexts(tableAdapter(app.getEngine().getAll(), font, size_font));
    tables["selected"]->setBodyFunc(true, true);

    tables["selected"]->filtredByRows(app.getEngine().getSelected());
    tables["filtred"]->filtredByRows(app.getEngine().getFiltred());
}

void SelectSchedule::handleInput(std::optional<sf::Event> event)
{
    for (auto& [name, btn] : buttons) {
        btn->handleEvent(*event);
    }
    for (auto& [name, table] : tables) {
        table->handleEvent(*event);
    }
}

void SelectSchedule::update(float dt)
{
    sf::RenderWindow& window = app.getWindow();
    auto mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    for (auto& [name, btn] : buttons) {
        btn->update(dt, mouse_pos);
    }
    for (auto& [name, table] : tables) {
        table->update(dt, mouse_pos);
    }
}

void SelectSchedule::draw(sf::RenderWindow& window)
{
    for (auto& [name, btn] : buttons) {
        window.draw(*btn);
    }
    for (auto& [name, table] : tables) {
        window.draw(*table);
    }
}

void SelectSchedule::buttonSelect()
{
    app.getEngine().selectByIndex(tables["filtred"]->getSelected().row);
    tables["selected"]->filtredByRows(app.getEngine().getSelected());
    tables["filtred"]->filtredByRows(app.getEngine().getFiltred());
}

void SelectSchedule::buttonUnselect()
{
    app.getEngine().unselectByIndex(tables["selected"]->getSelected().row);
    tables["selected"]->filtredByRows(app.getEngine().getSelected());
    tables["filtred"]->filtredByRows(app.getEngine().getFiltred());
}

void SelectSchedule::buttonCompletedMatter()
{
    app.getEngine().completeMatter(tables["filtred"]->getSelected().row);
    tables["filtred"]->filtredByRows(app.getEngine().getFiltred());
}

void SelectSchedule::buttonSchedule()
{
    app.changeState(std::make_unique<Schedule>(app));
}

void SelectSchedule::buttonMatters()
{
    app.changeState(std::make_unique<Matters>(app));
}

void SelectSchedule::buttonSave()
{
    app.getEngine().save("../resources/saves/1.json");
    tables["selected"]->filtredByRows(app.getEngine().getSelected());
    tables["filtred"]->filtredByRows(app.getEngine().getFiltred());
}

void SelectSchedule::buttonLoad()
{
    app.getEngine().load("../resources/saves/1.json");
    tables["selected"]->filtredByRows(app.getEngine().getSelected());
    tables["filtred"]->filtredByRows(app.getEngine().getFiltred());
}