#include "Client.h"
#include <iostream>
#include <string>

int main() {
    const std::string server_address = "127.0.0.1";
    const int port = 54000;

    try {
        Client client(server_address, port);
        client.start();

        std::string message;
        while (client.isRunning()) {
            std::getline(std::cin, message);
            client.sendMessage(message);
        }

        client.stop();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
