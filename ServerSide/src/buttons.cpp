#include "buttons.hpp"

Button::Button(int x, int y, int width, int height, std::string text) {
    button.setSize(sf::Vector2f(width, height));
    button.setPosition(x, y);
    button.setFillColor(sf::Color::Green);
    this->text.setString(text);
    this->text.setPosition(x+10, y+10);
    this->text.setColor(sf::Color::Black);
    font.loadFromFile("fonts/ARIAL.TTF");
    this->text.setFont(font);
}

void Button::render(sf::RenderWindow &window) {
    window.draw(button);
    window.draw(text);
}

bool Button::tryToClick(sf::RenderWindow &window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if(button.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        return true;
    } else {
        return false;
    }
    
}





