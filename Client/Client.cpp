#include "Client.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

Client::Client(const std::string& server_address, int port) : client_sock(-1), running(false) {
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock == -1) {
        std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, server_address.c_str(), &server_addr.sin_addr);
    server_addr.sin_port = htons(port);

    if (connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Connection failed: " << strerror(errno) << std::endl;
        close(client_sock);
        exit(EXIT_FAILURE);
    }

    std::cout << "Enter your username: ";
    std::getline(std::cin, username);
}

Client::~Client() {
    stop();
}

void Client::start() {
    running = true;
    receive_thread = std::thread(&Client::receiveMessages, this);
}

void Client::stop() {
    running = false;
    if (client_sock != -1) {
        close(client_sock);
        client_sock = -1;
    }
    if (receive_thread.joinable()) {
        receive_thread.join();
    }
}

void Client::sendMessage(const std::string& message) {
    if (client_sock != -1) {
        std::string full_message = username + ": " + message;
        send(client_sock, full_message.c_str(), full_message.size(), 0);
    }
}

void Client::receiveMessages() {
    char buffer[1024];
    int iResult;

    while (running) {
        iResult = recv(client_sock, buffer, sizeof(buffer), 0);
        if (iResult > 0) {
            buffer[iResult] = '\0';
            std::cout << buffer << std::endl;
        }
        else if (iResult == 0) {
            std::cout << "Connection closed by server\n";
            stop();
            break;
        }
        else {
            std::cerr << "recv failed: " << strerror(errno) << std::endl;
            stop();
            break;
        }
    }
}

void Client::setUsername(const std::string& username) {
    this->username = username;
}
