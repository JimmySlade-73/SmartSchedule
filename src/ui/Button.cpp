#include "ui/Button.hpp"
#include <iostream>

Button::Button()
{
    
}

void Button::rebuildTextures() const
{
    sf::Vector2u s(
        static_cast<unsigned int>(size.x),
        static_cast<unsigned int>(size.y)
    );
    if (!normal_rt.resize(s)) {
        std::cout << "error al crear normal_rt" << std::endl;
    }
    if (!hover_rt.resize(s)) {
        std::cout << "error al crear hover_rt" << std::endl;
    }
    normal_rt.clear();
    hover_rt.clear();

    sf::RectangleShape rect;
    rect.setSize({size.x  - 2 * outline_thickness, size.y - 2 * outline_thickness});
    rect.setPosition({outline_thickness, outline_thickness});
    //label
    auto label_rect = label->getLocalBounds();
    label->setOrigin({
        label_rect.position.x + label_rect.size.x / 2,
        label_rect.position.y + label_rect.size.y / 2
    });
    label->setPosition({
        rect.getPosition().x + rect.getSize().x / 2,
        rect.getPosition().y + rect.getSize().y / 2
    });

    //draw
    rect.setOutlineColor(outline_color);
    rect.setOutlineThickness(outline_thickness);

    rect.setFillColor(normal_color);
    normal_rt.draw(rect);
    normal_rt.draw(*label);
    
    rect.setFillColor(hover_color);
    hover_rt.draw(rect);
    hover_rt.draw(*label);

    normal_rt.display();
    hover_rt.display();
    is_dirty = false;
}

/// @brief Actualiza la logica del boton.
/// @param dt el diferencial de tiempo.
/// @param mouse_pos posicion del mouse.
void Button::update(float dt, sf::Vector2f mouse_pos)
{
    sf::FloatRect bounds({0.f, 0.f}, size);
    is_hovered = getTransform().transformRect(bounds).contains(mouse_pos);
    is_activated = is_hovered;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    
    if (is_dirty) { rebuildTextures(); }
    sf::Sprite sprite(is_activated ? hover_rt.getTexture() : normal_rt.getTexture());
    target.draw(sprite, states);
}

void Button::handleEvent(const sf::Event& event)
{
    if (auto* clicked = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (clicked->button == sf::Mouse::Button::Left && is_hovered) {
            if (onClick) {
                onClick();
            } else {
                std::cout << "the funtion doesn't exist" << std::endl;
            }
        }
    }
}
