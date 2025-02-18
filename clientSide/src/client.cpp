#include "client.hpp"
client::client() {
    logl("client created");
}


void client::connect(sf::IpAddress ip, int port) {
    this->ip = ip;
    this->port = port;
    logl("Connecting to server");
	sf::Socket::Status status = socket.connect(ip, port);
    if (status != sf::Socket::Done)
    {
        logl("Error in connecting to server");
		logl("Error code: " << status);
    }
    else {  
        logl("connected to server on ip: " << ip << " port: " << port);
        isConnected = true;
		std::thread receiveThread(&client::receivePackets, this);
		receiveThread.detach();
    }
}




bool client::isConnectedToServer() const{
	return isConnected;
}



void client::receivePackets() {
	sf::Packet packet;
	while (isConnected) {
		if (socket.receive(packet) == sf::Socket::Disconnected) {
			logl("Server disconnected");
			disconnect();
			break;
		}
		if (packet.getDataSize() != 0) {
			std::string message;
			packet >> message;
			logl(message);
			packet.clear();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	logl("receivePackets thread stopped");
}

void client::sendPacket(sf::Packet comingPacket) {
	std::string message;
	comingPacket >> message;
	if (message.size() == 0) {
		return;
	}
	sf::Packet packet;
	packet << message;
	socket.send(packet);
}

void client::disconnect() {
	isConnected = false;
	// socket.disconnect(); disconnects the socket but the server still thinks the client is connected.
	// i will send a packet to the server to tell it that the client is disconnecting.
	sf::Packet packet;
	packet << "disconnect:845981";
	socket.send(packet);
	isConnected = false;
	socket.disconnect();
	
	logl("Disconnected from server");
}