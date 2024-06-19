#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <thread>
#include <atomic>
#include <netinet/in.h> // pentru struct sockaddr_in
#include <unistd.h>     // pentru close
#include <mutex>

class Server;

class ClientHandler {
public:
    ClientHandler(int client_sock, Server* server);
    ~ClientHandler();

    void start();
    void stop();
    void sendMessage(const std::string& message);
    int getClientSock() const { return client_sock; }

private:
    void handleClient();

    int client_sock;
    Server* server;
    std::atomic<bool> running;
    std::thread client_thread;
    std::mutex send_mutex;
};

#endif // CLIENT_HANDLER_H
