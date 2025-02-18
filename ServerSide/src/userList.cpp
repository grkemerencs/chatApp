#include "userList.hpp"
#include "log.hpp"

userList::userList(std::vector<sf::TcpSocket*>* clients, std::unordered_map<sf::TcpSocket*, std::string>* nicknames,
int lowerX, int upperX, int lowerY, int upperY, int port) :
    clients(clients), nicknames(nicknames), lowerX(lowerX), upperX(upperX), lowerY(lowerY), upperY(upperY)
{
    font.loadFromFile("fonts/ARIAL.TTF");
    text.setFont(font);
    text.setColor(sf::Color::Black);
    text.setCharacterSize(20);
    background.setSize(sf::Vector2f(upperX-lowerX, upperY-lowerY));
    background.setFillColor(sf::Color::Green);
    background.setPosition(lowerX, lowerY);
    serverAdress.setFont(font);
    serverAdress.setColor(sf::Color::Black);
    serverAdress.setCharacterSize(30);
    serverAdress.setString(sf::IpAddress::getLocalAddress().toString()+ " : " + std::to_string(port));
    serverAdress.setPosition(lowerX,770);
}


void userList::render(sf::RenderWindow &window){
    window.draw(background);
    for(int i = 0; i < clients->size(); i++){
        text.setString(nicknames->at(clients->at(i)) + " : " + clients->at(i)->getRemoteAddress().toString());
        text.setPosition(lowerX+10, lowerY+ 10 + i*30);
        if(clients->at(i) == selectedClient){
            text.setColor(sf::Color::Red);
            window.draw(text);
            text.setColor(sf::Color::Black);
            continue;
        }
        window.draw(text);
    }
    window.draw(serverAdress);
}

void userList::updateListFromClients() {
    clientsTexts.clear();
    try {
        for(int i = 0; i < clients->size(); i++){
        text.setString(nicknames->at(clients->at(i)) + " : " + clients->at(i)->getRemoteAddress().toString());
        text.setPosition(lowerX+10, lowerY+ 10 + i*20);
        clientsTexts.push_back(text);
        }
    } catch (const std::exception& e) { // in case a client dissconnects while updating the list.
        error(e.what());
        clientsTexts.clear();
    }
}


void userList::selectClientWithMause(sf::RenderWindow &window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    for(int i = 0; i < clientsTexts.size(); i++){
        if(clientsTexts[i].getGlobalBounds().contains(mousePos.x, mousePos.y)){
            selectedClient = clients->at(i);
            return;
        }
    }
    selectedClient = nullptr;
    render(window);
}

sf::TcpSocket* userList::getSelectedClient() {
    return selectedClient;
}



