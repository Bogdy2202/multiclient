#include <iostream>

#include "Client.h"

int main() {
    const std::string server_address = "127.0.0.1";
    const int port = 54000;

    Client client(server_address, port);
    client.start();

    std::string message;
    while (client.isRunning()) {
        std::getline(std::cin, message);
        client.sendMessage(message);
    }

    client.stop();

    return 0;
}
