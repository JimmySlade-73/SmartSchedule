#pragma once
#include <SFML/Graphics.hpp>

class App;

class State {
protected:
    App& app;
public:
    State(App& app_instance) : app(app_instance) {}
    virtual ~State() = default;
    
    virtual void handleInput(std::optional<sf::Event> event) = 0;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};