#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <thread>
#include <atomic>
#include <netinet/in.h> // pentru struct sockaddr_in
#include <arpa/inet.h>  // pentru inet_pton
#include <unistd.h>     // pentru close

class Client {
public:
    Client(const std::string& server_address, int port);
    ~Client();

    void start();
    void stop();
    void sendMessage(const std::string& message);
    void sendMessages();
    void setUsername(const std::string& username);
    bool GetRunning();

private:
    void receiveMessages();

    int client_sock;
    struct sockaddr_in server_addr;
    std::thread receive_thread;
    std::atomic<bool> running;
    std::string username;
};

#endif // CLIENT_H
