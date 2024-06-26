#include <iostream>

#include "Client.h"

int main() {
    const std::string server_address = "127.0.0.1";
    const int port = 54000;

    Client client(server_address, port);
    client.start();

    std::string message;
    std::cin.ignore();  // Ignore newline left in the buffer

    std::cout << "You can start typing messages. Type 'exit' to quit." << std::endl;

    while (true) {
        std::getline(std::cin, message);

        if (message == "exit") {
            break;
        }

        client.sendMessage(message);
    }

    client.stop();

    return 0;
}
