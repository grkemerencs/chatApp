#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <unordered_map>

class userList {
    private:
        int lowerY;
        int upperY;
        int lowerX;
        int upperX;
        std::vector<sf::TcpSocket*>* clients; //set clients from serverUtil which will be listed.
        std::unordered_map<sf::TcpSocket*, std::string>* nicknames; //set nicknames from serverUtil which will be listed.
        sf::Font font;
        std::vector<sf::Text> clientsTexts;
        sf::Text text; // example text for adding new ones.
        sf::RectangleShape background;
        sf::TcpSocket* selectedClient = nullptr;
        sf::Text serverAdress;
    public:
        userList(std::vector<sf::TcpSocket*>* clients, std::unordered_map<sf::TcpSocket*, std::string>* nicknames,
        int lowerX, int upperX, int lowerY, int upperY, int port);

        void updateListFromClients();
        void selectClientWithMause(sf::RenderWindow &window);
        void render(sf::RenderWindow &window);
        void kickClient(std::string ip);
        sf::TcpSocket* getSelectedClient();
};