#ifndef LAYOUT_H
#define LAYOUT_H

#include <SFML/System/Vector2.hpp>
#include <vector>


enum class LayoutType
{
    HORIZONTAL,
    VERTICAL
};

class Layout
{
private:
    sf::Vector2f pos;
    sf::Vector2f size;

    LayoutType type;
    unsigned int padding;

    std::vector<Layout> children;

public:
    Layout(sf::Vector2f pos, LayoutType type) : pos(pos), type(type) {}

    void add_child(sf::Vector2f child_witdh) {}

    sf::Vector2f get_pos() const
    {
        return pos;
    }

    unsigned int get_padding() const
    {
        return padding;
    }

    unsigned int get_width() const
    {
        return padding * 2 + 1;
    }
};

class Element
{
protected:
    Layout layout;

    virtual void set_layout(Layout layout)
    {
        this->layout = layout;
    }
};
#endif // !LAYOUT_H
