#include "Server.h"

int main() {
    const int port = 54000;
    Server server(port);

    server.start();

    return 0;
}
