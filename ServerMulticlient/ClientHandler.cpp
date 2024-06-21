#include "ClientHandler.h"
#include "Logger.h"
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>

#include "Server.h"

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
            Logger::getInstance().log("Received from client " + std::to_string(client_sock) + ": " + message);

            // Forward message to all clients
            server->broadcastMessage(message);
        } else if (iResult == 0) {
            Logger::getInstance().log("Client " + std::to_string(client_sock) + " disconnected.");
            break;
        } else {
            Logger::getInstance().log("recv failed from client " + std::to_string(client_sock) + ": " + strerror(errno));
            break;
        }
    }

    close(client_sock);
}

void ClientHandler::sendMessage(const std::string& message) {
    if (client_sock != -1) {
        send(client_sock, message.c_str(), message.size(), 0);
        Logger::getInstance().log("Sent to client " + std::to_string(client_sock) + ": " + message);
    }
}
