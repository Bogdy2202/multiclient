#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <memory>
#include <netinet/in.h> // pentru struct sockaddr_in
#include "ClientHandler.h"

class Server {
public:
    Server(int port);
    ~Server();

    void start();
    void stop();
    void broadcastMessage(const std::string& message);
    void removeClientHandler(ClientHandler* handler);
    void sendPrivateMessage(const std::string& username, const std::string& message);
    std::vector<std::string> getUsernames() const;

private:
    void acceptClients();

    int listen_sock;
    struct sockaddr_in saServer;
    std::vector<std::unique_ptr<ClientHandler>> clientHandlers;
    std::atomic<bool> running;
};

#endif // SERVER_H
