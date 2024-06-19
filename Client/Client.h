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
    bool isRunning() const { return running; }

private:
    void receiveMessages();

    int client_sock;
    sockaddr_in server_addr;
    std::atomic<bool> running;
    std::thread receive_thread;
};

#endif // CLIENT_H
