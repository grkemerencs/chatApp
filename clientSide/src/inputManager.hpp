#pragma once
#include "client.hpp"
#include <iostream>
#include <thread>
#include <sstream>
#define logl(x) std::cout << x << std::endl

class inputManager  {
    private:
        client *currentClient;
        bool running = false;
    public:
        inputManager();
        void getInputFromUser();
        void setClient(client &client);
        void run();
        void stop();
};
