#include "Server.h"
#include <iostream>

int main() {
    const int port = 54000;

    try {
        Server server(port);
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
