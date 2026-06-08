#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../../include/logic/ScheduleEngine.hpp"
#include "../../include/states/State.hpp"

class App {
private:
    sf::RenderWindow window;
    ScheduleEngine engine;
    std::unique_ptr<State> currentState;
    std::unique_ptr<State> pendingState;

public:
    App();
    void run();
    sf::RenderWindow& getWindow() { return window; }
    sf::Vector2f getSize() { return sf::Vector2f(window.getSize()); }
    sf::Vector2f getSizeByPercent(float a, float b) { return {getSize().x * a, getSize().y * b}; }
    void changeState(std::unique_ptr<State> newState);
    ScheduleEngine& getEngine() { return engine; }
};