#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include <netinet/in.h> // pentru struct sockaddr_in
#include <mutex>
#include "ClientHandler.h"

class Server {
public:
    Server(int port);
    ~Server();

    void start();
    void stop();
    void broadcastMessage(const std::string& message, int sender_sock);

private:
    void acceptClients();

    int listen_sock;
    sockaddr_in saServer;
    std::atomic<bool> running;
    std::vector<std::unique_ptr<ClientHandler>> clientHandlers;
    std::mutex clients_mutex;
};

#endif // SERVER_H
