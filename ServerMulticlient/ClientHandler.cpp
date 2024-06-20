#include "ClientHandler.h"
#include "Server.h"
#include <iostream>
#include <unistd.h>
#include <cstring>

ClientHandler::ClientHandler(int client_sock, Server* server) : client_sock(client_sock), running(true), server(server) {
    client_thread = std::thread(&ClientHandler::handleClient, this);
}

ClientHandler::~ClientHandler() {
    stop();
    if (client_thread.joinable()) {
        client_thread.join();
    }
}

void ClientHandler::start() {
    running = true;
}

void ClientHandler::stop() {
    running = false;
    if (client_sock != -1) {
        close(client_sock);
    }
}

void ClientHandler::handleClient() {
    char buffer[1024];
    int iResult;

    while (running) {
        iResult = recv(client_sock, buffer, sizeof(buffer), 0);
        if (iResult > 0) {
            buffer[iResult] = '\0';
            std::string message(buffer);
            std::cout << "Received: " << message << std::endl;

            // Forward message to all clients
            server->broadcastMessage(message);
        }
        else if (iResult == 0) {
            std::cout << "Connection closed\n";
            break;
        }
        else {
            std::cerr << "recv failed: " << strerror(errno) << std::endl;
            break;
        }
    }

    close(client_sock);
}

void ClientHandler::sendMessage(const std::string& message) {
    if (client_sock != -1) {
        send(client_sock, message.c_str(), message.size(), 0);
    }
}
