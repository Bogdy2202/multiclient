#include "ClientHandler.h"
#include "Server.h"
#include <iostream>
#include <cstring> // pentru memset

ClientHandler::ClientHandler(int client_sock, Server* server) : client_sock(client_sock), server(server), running(true) {
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
        client_sock = -1;
    }
}

void ClientHandler::sendMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(send_mutex);
    if (client_sock != -1) {
        send(client_sock, message.c_str(), message.size(), 0);
    }
}

void ClientHandler::handleClient() {
    char buffer[1024];
    int iResult;

    while (running) {
        iResult = recv(client_sock, buffer, sizeof(buffer), 0);
        if (iResult > 0) {
            buffer[iResult] = '\0';
            std::cout << "Received from client: " << buffer << std::endl;
            server->broadcastMessage(buffer, client_sock);
        } else if (iResult == 0) {
            std::cout << "Connection closed by client\n";
            stop();
            break;
        } else {
            std::cerr << "recv failed: " << strerror(errno) << std::endl;
            stop();
            break;
        }
    }

    close(client_sock);
}
