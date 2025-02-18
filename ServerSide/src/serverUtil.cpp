#include "serverUtil.h"
Server::Server(int port, sf::IpAddress ip)
	: port(port), ip(ip)
{

	if (listener.listen(port,ip) != sf::Socket::Done)
	{
		logl("Error in starting server");
	}
	else
	{
		logl("Server started on port " + port);
	}
}

void Server::setChat(chat* chat) {
	mchat = chat;
}

void Server::setUserList(userList* userList) {
	muserList = userList;
}

std::vector<sf::TcpSocket*>* Server::getClients() {
	return &clients;
}
std::unordered_map<sf::TcpSocket*, std::string>* Server::getNicknames() {
	return &nicknames;
}


int Server::getPort() {
	return port;
}

sf::IpAddress Server::getIp() {
	return ip;
}


void Server::acceptClients() {
	std::string ipAsStr = ip.toString();
	sf::TcpSocket* client = new sf::TcpSocket;
	sf::Packet packet;
	listener.setBlocking(false);
	while (isServerRunning) {
		sf::Socket::Status status = listener.accept(*client);
		if (status == sf::Socket::Done)
		{
			client->setBlocking(false);
			clients.push_back(client);
			nicknames[client] = "Unknown";
			logl("Client connected on ip :" + ipAsStr);
			packet << "Welcome to the chat\n you need to set your nickname by typing set_name <name> \nyou can type commands to see all server commands";
			client->send(packet);
			packet.clear();
			client = new sf::TcpSocket;
			muserList->updateListFromClients();
		} else if (status == sf::Socket::Disconnected) {
			delete client;
			client = new sf::TcpSocket;
			logl("Error in accepting client");
			logl("Client ip: " + ip.toString());
		} else if (status == sf::Socket::NotReady) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		} else {
			logl("unexpected Error in accepting client");
			logl("Client ip: " + ip.toString());
		}
	}
	logl("acceptClients thread stopped");
}


int Server::disconnectClient(int index) {
	if (index < 0 || index >= clients.size()) {
		error("Invalid index");
		return -1;
	}
	sf::TcpSocket *client = clients[index];
	client->disconnect();
	nicknames.erase(client);
	delete client;
	clients.erase(clients.begin() + index);
	muserList->updateListFromClients();
	return 0;
}
int Server::disconnectClient(sf::TcpSocket* client) {
	for(int i = 0; i < clients.size(); i++) {
		if(clients[i] == client) {
			return disconnectClient(i);
		}
	}
	return -1;
}


void Server::broadcastPacket(sf::Packet &packet,const std::string& nickname) const {
	std::string message;
	packet >> message;
	logl("broadcasting message: " + message);
	message.insert(0, nickname + ": ");
	for (int i = 0; i < clients.size(); i++) {
		sf::Packet sendPacket;
		sendPacket << message;
		clients[i]->send(sendPacket);
	}
}


void Server::receivePackets() {
	while (isServerRunning) {
		sf::Packet packet;
		for (int i = 0; i < clients.size(); i++) {
			sf::Socket::Status status = clients[i]->receive(packet);
			// this doest see the socket.disconnect() but it sees the client close the app without /exit
			if (status == sf::Socket::Disconnected) {
				logl("status disconnected");
				disconnectClient(i);
				i--;
				continue;
			}
			if(packet.getDataSize() == 0) { // since client setblocking to false, it will return empty packet if there is no data and we need to skip it.
				continue;
			}
			std::string message;
			packet >> message;
			std::stringstream stream(message);
			std::string command;
			stream >> command;
			
			if(command == "disconnect:845981") {
				disconnectClient(i);
				i--;
				continue;
			}
			if(command == "set_name") {
				std::string nickname;
				stream >> nickname;
				if(nickname.empty()) {
					packet.clear();
					packet << "Invalid nickname";
					clients[i]->send(packet);
					packet.clear();
					continue;
				}
				nicknames[clients[i]] = nickname;
				packet.clear();
				continue;
			}
			if (command == "list") {
				std::string list = "Clients: ";
				for (int j = 0; j < clients.size(); j++) {
					list += nicknames.at(clients[j]) + "\n";
				}
				packet.clear();
				packet << list;
				clients[i]->send(packet);
				packet.clear();
				continue;
			}
			if(command == "commands") {
				std::string commands = "Commands: \n";
				commands += "disconnect - to disconnect the chat\n";
				commands += "list - to list all clients\n";
				commands += "set_name <name> - to change nickname\n";
				packet.clear();
				packet << commands;
				clients[i]->send(packet);
				packet.clear();
				continue;
			}
			if(nicknames.count(clients[i]) == 0) {
				packet.clear();
				packet << "You need to set your nickname by typing set_name <name>";
				clients[i]->send(packet);
				packet.clear();
				continue;
			}
			sf::Packet packet2;
			packet2 << message;
			mchat->addMessage(nicknames[clients[i]] + ": " + message);
			broadcastPacket(packet2, nicknames[clients[i]]);
			packet.clear();
			
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	logl("receivePackets thread stopped");



}



void Server::start() {
	isServerRunning = true;
	acceptClientsThread = std::thread(&Server::acceptClients, this);
	receivePacketsThread = std::thread(&Server::receivePackets, this);
	//there was input system but it was removed.
}