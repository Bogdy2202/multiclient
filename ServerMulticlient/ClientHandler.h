#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <thread>
#include <atomic>
#include <string>

class Server;  // Forward declaration

class ClientHandler {
public:
    ClientHandler(int client_sock, Server* server);
    ~ClientHandler();

    void start();
    void stop();
    void sendMessage(const std::string& message);

private:
    void handleClient();

    int client_sock;
    std::thread client_thread;
    std::atomic<bool> running;
    Server* server;  // Pointer to server
};

#endif // CLIENTHANDLER_H
