#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <SFML/Window.hpp>
#include "chat.hpp"
#include <thread>
#include <chrono>
#include "buttons.hpp"

void renderWindow(sf::RenderWindow &window, chat &mchat, userList &muserList, Button &mbutton);

int main()
{   int width = 1200;
    int height = 800;
    sf::RenderWindow  window(sf::VideoMode(width, height), "Chat App", sf::Style::Titlebar | sf::Style::Close);
    chat mchat(20, 800, 20, 600);
    Server mserver(55428, sf::IpAddress::getLocalAddress());
    Button mbutton(900, 700, 100, 50, "kick");
    logl("server starting on port: " + std::to_string(mserver.getPort()) + " ip: " + mserver.getIp().toString());
    userList muserList(mserver.getClients(), mserver.getNicknames(), 820, width-20 , 20 , height - 300, mserver.getPort());
    mserver.setChat(&mchat);
    mserver.setUserList(&muserList);
    mserver.start();
    mchat.addMessage("welcome to the chat app");
    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            if(!window.hasFocus()) { // if window dont have the focus dont get keyboard inputs.
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if(event.type == sf::Event::TextEntered || event.type == sf::Event::KeyPressed){
                mchat.chatBarInput(event,mserver);
            }
            if(event.type == sf::Event::MouseButtonPressed){
                if(mbutton.tryToClick(window)){
                    sf::TcpSocket* selectedClient = muserList.getSelectedClient();
                    if(selectedClient != nullptr){
                        mserver.disconnectClient(selectedClient);
                    }
                } else {
                muserList.selectClientWithMause(window);
                }
            }
            renderWindow(window, mchat, muserList, mbutton);
            
        }
    
    }
    renderWindow(window, mchat, muserList, mbutton);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void renderWindow(sf::RenderWindow &window, chat &mchat, userList &muserList, Button &mbutton) {
    window.clear(sf::Color::White);
    mchat.chatRender(window);
    muserList.render(window);
    mbutton.render(window);
    window.display();
}