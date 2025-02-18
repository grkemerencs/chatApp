#include "inputManager.hpp"


inputManager::inputManager() {
    logl("inputManager created");
}


void inputManager::setClient(client &client) {
    currentClient = &client;
};

void inputManager::getInputFromUser() {
    std::string message;
    logl("Commands:\n /exit - stops the application. \n/connect <ip> <port> - connects to the server. \n/disconnect - disconnects from the server.");
    while (running) {
        std::getline(std::cin, message);
        std::stringstream ss;
        ss << message;
        std::string command;
        ss >> command;
        if (command == "/exit") {
            running = false;
            currentClient->disconnect();
            break;
        }
        if (command == "/connect") {
            if (currentClient->isConnectedToServer()) {
                logl("Already connected to server");
                continue;
            }
            std::string str_ip;
            int port;
            ss >> str_ip >> port;
            if(str_ip == "l") { // localhost
                str_ip = "127.0.0.1";
            }
            try {
                sf::IpAddress ip(str_ip);
                logl("ip is " << ip);
                logl("port is " << port);
                currentClient->connect(ip, port);
            } catch (std::exception e) {
                logl("Invalid ip address or port");
            }
            continue;
        }
        if (command == "/disconnect") {
            sf::Packet packet;
            // stop the client.
            if(currentClient->isConnectedToServer()) {
                currentClient->disconnect();
            } else {
                logl("Not connected to server yet use /connect <ip> <port> to connect to a server");
            }
            continue;
        }
        if (command.front() == '/') {
            logl("Invalid command");
            logl("Commands:\n /exit - stops the application. \n/connect <ip> <port> - connects to the server. \n/disconnect - disconnects from the server.");
            logl("you can see server commands by typing commands without the /");
            logl("To send a message type the message and press enter.");
            
            continue;
        }
        if (currentClient->isConnectedToServer()) {
            sf::Packet packet;
            packet << message;
            currentClient->sendPacket(packet);
        }
        else {
            logl("Not connected to server yet use /connect <ip> <port> to connect to a server");
        }
    }
    
}
void inputManager::run() {
    running = true;
    this->getInputFromUser();
}

void inputManager::stop() {
    running = false;
}

