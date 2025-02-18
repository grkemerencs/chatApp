
#include "log.hpp"

void logl(std::string message) {
    std::cout << "Server Message: " << message << std::endl;
}

void error(std::string message) {
    std::cout << "Error: " << message << std::endl;
}

void warning(std::string message) {
    std::cout << "Warning: " << message << std::endl;
}