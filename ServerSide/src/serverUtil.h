#pragma once
#include <iostream>
#include <SFML/Network.hpp>
#include <vector>
#include <string>
#include <thread>
#include <unordered_map>
#include <sstream>
#include "log.hpp"
#include "chat.hpp"
#include "userList.hpp"

class chat;
class userList;

class Server {
private:
	bool debug = false;
	std::thread acceptClientsThread;
	std::thread receivePacketsThread;
	bool isServerRunning;
	int port;
	sf::IpAddress ip;
	// list of clients
	std::vector<sf::TcpSocket*> clients;
	std::unordered_map<sf::TcpSocket*, std::string> nicknames;
	// listener socket
	sf::TcpListener listener;
	chat* mchat;
	userList* muserList;
	
public:

	Server(int port, sf::IpAddress ip);
    void setChat(chat *chat);
    void setUserList(userList *userList);
    std::vector<sf::TcpSocket *>* getClients();
    std::unordered_map<sf::TcpSocket *, std::string>* getNicknames();
    int getPort();
    sf::IpAddress getIp();
    // accept clients and add them to a list do it in while loop and give thread.sleep(1000) to avoid cpu usage
    void acceptClients();
	// disconnetc the given client and remove it from the list.
	int disconnectClient(int index);  // return 0 if success -1 if fail.
	int disconnectClient(sf::TcpSocket* client);
	// broadcast the packet to all clients.
	void broadcastPacket(sf::Packet &packet, const std::string& nickname) const;
	// accept packets from the client.
	void receivePackets();
	void StartCommandInput();
	void start();
};