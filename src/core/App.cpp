#include "core/App.hpp"
#include "states/SelectSchedule.hpp"
#include <iostream>

App::App() {
    window.create(sf::VideoMode({1280, 720}), "Horario Dinamico");
    engine.loadData("../resources/data/data.txt");
    
    currentState = std::make_unique<SelectSchedule>(*this);
}

void App::changeState(std::unique_ptr<State> newState) {
    pendingState = std::move(newState);
}

void App::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            currentState->handleInput(event);
        }
        
        if (pendingState) {
            currentState = std::move(pendingState);
            pendingState = nullptr;
        }
        
        currentState->update(dt);

        window.clear(sf::Color(30, 30, 30));
        currentState->draw(window);
        window.display();
    }
}