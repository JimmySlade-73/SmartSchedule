#include "ui/TableWidget.hpp"
#include <iostream>

void TableWidget::applyLayout(
    std::vector<std::unique_ptr<Button>>& row, float y_offset)
{
    if(row.size() != proportions.size()) {
        std::cout << "error de tamano" << std::endl;
        return;
    }
    float total = 0;
    for (const int col : visible_cols) {
        total += proportions.at(col);
    }
    float sum = 0;
    for (int i = 0; i < visible_cols.size(); i++) {
        int col = visible_cols.at(i);
        auto outline_thickness = row.at(col)->getOutlineThickness();
        float width = size.x - outline_thickness;
        row.at(col)->setPosition({width * sum / total, y_offset});
        row.at(col)->setSize({width * proportions[col] / total + outline_thickness, size_row});
        sum += proportions[col];
    }
}

void TableWidget::setTexts(std::vector<std::vector<sf::Text>> texts)
{
    records.clear();
    visible_rows.clear();
    for (int i = 0; i < texts.size(); i++) {
        if(texts.at(0).size() != texts.at(i).size()) {
            std::cout << "error de tamano" << std::endl;
            return;
        }
        if (!is_load_rows) visible_rows.push_back(i);
    }
    for (auto& row : texts) {
        std::vector<std::unique_ptr<Button>> btn_row;
        for (auto& text : row) {
            auto btn = std::make_unique<Button>();
            btn->setText(text);
            btn_row.push_back(std::move(btn));
        }
        records.push_back(std::move(btn_row));
    }
    is_dirty = true;
}

void TableWidget::setHeaders(std::vector<sf::Text> h)
{
    for (const auto& text : h) {
        auto btn = std::make_unique<Button>();
        btn->setText(text);
        headers.push_back(std::move(btn));
    }
}

void TableWidget::setProportions(std::vector<float> p)
{
    proportions = p;
    is_dirty = true;
    visible_cols.clear();
    if (is_load_cols) return;
    for (int i = 0; i < p.size(); i++) {
        visible_cols.push_back(i);
    }
}

void TableWidget::setBodyFunc(bool select_r, bool select_c)
{
    for (int r = 0; r < records.size(); r++) {
        for (int c = 0; c < records.at(r).size(); c++) {
            std::function<void()> func;
            if (select_r && select_c) {
                func = [this, r, c](){ selected = {r, c}; activated = {r, c}; };
            } else if (select_r) {
                func = [this, r, c](){ selected = {r, c}; activated.row = r; };
            } else if (select_c) {
                func = [this, r, c](){ selected = {r, c}; activated.col = c; };
            }
            records.at(r).at(c)->setFunc(func);
        }
    }
}

void TableWidget::build()
{
    applyLayout(headers, 0.f);
    for (int i = 0; i < visible_rows.size(); i++) {
        applyLayout(records.at(visible_rows.at(i)), i * size_row + scroll_offset);
    }
    
    if (!head_rt.resize({(unsigned int)size.x, (unsigned int)size_row})) {
        std::cout << "error al crear hover_rt" << std::endl;
    }
    float total_height = size.y - size_row;
    if (!body_rt.resize({(unsigned int)size.x, (unsigned int)total_height})) {
        std::cout << "error al crear hover_rt" << std::endl;
    }
    
    head_rt.clear();
    body_rt.clear(sf::Color::Transparent);
    
    for (const auto& col : visible_cols) {
        head_rt.draw(*headers.at(col));
    }
    int start = (unsigned int)(- scroll_offset / size_row);
    int end = (unsigned int)((- scroll_offset + size.y - size_row) / size_row + 0.999f);
    end = std::min(end, (int)visible_rows.size());
    for (int i = start; i < end; i++) {
        for (int j = 0; j < visible_cols.size(); j++) {
            int row = visible_rows.at(i);
            int col = visible_cols.at(j);
            body_rt.draw(*records.at(row).at(col));
        }
    }
    
    head_rt.display();
    body_rt.display();
    is_dirty = false;
}

void TableWidget::update(float dt, sf::Vector2f mouse_pos)
{
    sf::Vector2f local_mouse = getInverseTransform().transformPoint(mouse_pos);
    
    is_inside = local_mouse.y > 0 && local_mouse.y < size.y &&
    local_mouse.x > 0 && local_mouse.x < size.x;
    
    for (auto& col : visible_cols) {
        std::unique_ptr<Button>& btn = headers.at(col);
        bool was_activated = btn->isActivated();
        if (col == activated.col) {
            btn->update(dt, {local_mouse.x, local_mouse.y});
            btn->setActivate(true);
        } else {
            btn->update(dt, {local_mouse.x, local_mouse.y});
        }
        if (btn->isActivated() != was_activated) is_dirty = true;
    }
    
    
    local_mouse.y -= size_row;
    int i = static_cast<int>( local_mouse.y / size_row);
    int start = (unsigned int)(- scroll_offset / size_row);
    int end = start + (unsigned int)(size.y / size_row);
    for (int i = start; i < visible_rows.size(); i++) {
        int row = visible_rows.at(i);
        for (auto& col : visible_cols) {
            bool was_activated = records.at(row).at(col)->isActivated();
            if (col == activated.col || row == activated.row) {
                records.at(row).at(col)->update(dt, {local_mouse.x, local_mouse.y});
                records.at(row).at(col)->setActivate(true);
            } else {
                records.at(row).at(col)->update(dt, {local_mouse.x, local_mouse.y});
            }
            if (records.at(row).at(col)->isActivated() != was_activated) is_dirty = true;
        }
    }
    if (is_dirty) { build(); }
}

void TableWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    sf::Sprite head_s(head_rt.getTexture());
    target.draw(head_s, states);
    
    sf::Sprite body_s(body_rt.getTexture());
    body_s.setPosition({0.f, size_row});
    target.draw(body_s, states);
}

void TableWidget::handleEvent(const sf::Event& event)
{
    if(auto* wheel = event.getIf<sf::Event::MouseWheelScrolled>())
    {
        if (is_inside) {
            scroll_offset += wheel->delta * scroll_speed;
            is_dirty = true;
        }
        float max_scroll = size.y - (visible_rows.size() + 1) * size_row;
        scroll_offset = std::clamp(scroll_offset, std::min(0.f, max_scroll), 0.f);
    }
    if (auto* clicked = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (clicked->button == sf::Mouse::Button::Left) {
            activated.col = -1;
            activated.row = -1;
            selected.col = -1;
            selected.row = -1;
        }
    }
    if (!is_inside) { return; }
    for (const auto& col : visible_cols) {
        headers.at(col)->handleEvent(event);
    }
    for (const auto& row : visible_rows) {
        for (const auto& col : visible_cols) {
            records.at(row).at(col)->handleEvent(event);
        }
    }
}