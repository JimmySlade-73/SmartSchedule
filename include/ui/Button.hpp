#pragma once
#include <functional>
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

class Button : public sf::Drawable, public sf::Transformable {
private:
    sf::Vector2f size;
    mutable std::unique_ptr<sf::Text> label;
    std::function<void()> onClick;
    bool is_hovered = false;
    bool is_activated = false;
    
    float outline_thickness = 1.f;
    sf::Color outline_color = sf::Color::White;
    sf::Color normal_color  = sf::Color(50, 50, 200);
    sf::Color hover_color   = sf::Color(80, 80, 255);
    mutable sf::RenderTexture normal_rt;
    mutable sf::RenderTexture hover_rt;
    mutable bool is_dirty = true;

public:
    Button();
    float getOutlineThickness() { return outline_thickness; }
    bool isHovered() { return is_hovered; }
    bool isActivated() { return is_activated; }
    void setFunc(std::function<void()> funct) { onClick = funct; };
    void setSize(sf::Vector2f s) { size = s; is_dirty = true; };
    void setText(sf::Text T) { label = std::make_unique<sf::Text>(T); is_dirty = true; };
    void rebuildTextures() const;
    void setActivate(const bool& activate) { is_activated = activate; };
    void update(float dt, sf::Vector2f mouse_pos);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void handleEvent(const sf::Event& event);
};