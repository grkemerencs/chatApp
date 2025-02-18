// Last Modified: Wed, 10 Mar 2021 00:00:00 GMT
// Description: Client side of the chat.
#include "client.hpp"
#include "inputManager.hpp"
#include <iostream>
#include <SFML/Network.hpp>
#include <string>
#include <thread>
#define logl(x) std::cout << x << std::endl
void receivePackets(sf::TcpSocket& socket);
void sendPacket(sf::TcpSocket& socket);

int main()
{
	client client;
	inputManager inputManager;
	inputManager.setClient(client);
	inputManager.run();

}




//conncet to server
// recieve packets whiel loop
// send packets
// disconnect