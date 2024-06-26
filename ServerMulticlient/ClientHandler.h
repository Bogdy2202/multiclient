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
    std::string getUsername();

private:
    void handleClient();
    void processCommand(const std::string& command);

    int client_sock;
    std::thread client_thread;
    std::atomic<bool> running;
    Server* server;  // Pointer to server
    std::string username;  // Username-ul clientului
};

#endif // CLIENTHANDLER_H
