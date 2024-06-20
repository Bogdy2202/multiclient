#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <memory>
#include <netinet/in.h>
#include <atomic>
#include "ClientHandler.h"

class Server {
public:
    Server(int port);
    ~Server();

    void start();
    void stop();
    void broadcastMessage(const std::string& message);

private:
    void acceptClients();

    int listen_sock;
    struct sockaddr_in saServer;
    std::vector<std::unique_ptr<ClientHandler>> clientHandlers;
    std::atomic<bool> running;
};

#endif // SERVER_H
