#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "serverUtil.h"


class Server;

class Button {
    protected:
        sf::RectangleShape button;
        sf::Text text;
        sf::Font font;


    public:
        Button(int x, int y, int width, int height, std::string text);
        void render(sf::RenderWindow &window);
        bool tryToClick(sf::RenderWindow &window);
};




