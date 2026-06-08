#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "ui/Button.hpp"
#include <memory>
#include <iostream>

struct Selection {
    int row = -1;
    int col = -1;
};

class TableWidget : public sf::Drawable, public sf::Transformable {
private:
    sf::Vector2f size;
    float size_row;
    std::vector<float> proportions;
    Selection selected;
    Selection activated;

    std::vector<std::unique_ptr<Button>> headers;
    std::vector<std::vector<std::unique_ptr<Button>>> records;
    std::vector<int> visible_rows;
    std::vector<int> visible_cols;
    sf::RenderTexture body_rt;
    sf::RenderTexture head_rt;
    float scroll_offset = 0.f;
    float scroll_speed = 30.f;
    bool is_dirty = true;
    bool is_inside = false;
    bool is_load_rows = false;
    bool is_load_cols = false;

    void applyLayout(std::vector<std::unique_ptr<Button>>& row, float y_offset);
    
public:
    TableWidget() {};
    void setSize(sf::Vector2f s) { size = s; is_dirty = true; }
    void setSizeRow(float s) { size_row = s; is_dirty = true; }
    void setTexts(std::vector<std::vector<sf::Text>> texts);
    void setHeaders(std::vector<sf::Text> h);
    void setProportions(std::vector<float> p);
    void setBodyFunc(bool active_r, bool active_c);
    Selection getSelected() { return selected; }
    void filtredByRows(std::vector<int> rows) { visible_rows = rows; is_load_rows = true; is_dirty = true; }
    void filtredByCols(std::vector<int> cols) { visible_cols = cols; is_load_cols = true; is_dirty = true; }
    void build();
    void update(float dt, sf::Vector2f mouse_pos);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void handleEvent(const sf::Event& event);
};