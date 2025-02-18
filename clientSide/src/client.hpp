
#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>

#define logl(x) std::cout << x << std::endl;

class client { 
    private:
        sf::IpAddress ip;
        int port;
        sf::TcpSocket socket;
        bool isConnected = false;
    public:
        client();
        void connect(sf::IpAddress ip, int port);
        bool isConnectedToServer() const;
        void sendPacket(sf::Packet comingPacket);
        // on a thread recieve packets.
        void receivePackets();
        void disconnect();



};