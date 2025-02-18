#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <SFML/Network.hpp>
#include "log.hpp"
#include "serverUtil.h"

class Server;

class chat{
    private:
        const int lowerY;
        const int upperY; 
        const int lowerX;
        const int upperX;
        int maxCharsInOneLine;
        std::vector<sf::Text> messages;
        sf::Font chatFont;
        sf::Text chatBar;
        int charSize = 20;
        void shiftMessages();
        bool running;
        sf::RectangleShape chatBackground;
        

    public:
        chat(int lowerX, int upperX,int lowerY,int upperY);
        void addMessage(std::string message);
        void chatRender(sf::RenderWindow &window);
        void chatBarStart();
        void chatBarInput(sf::Event event,Server &server);

};