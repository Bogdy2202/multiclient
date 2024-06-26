#include "ClientHandler.h"
#include "Logger.h"
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include "Server.h"

ClientHandler::ClientHandler(int client_sock, Server* server)
    : client_sock(client_sock), running(true), server(server) {
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

    // Receiving username from the client
    iResult = recv(client_sock, buffer, sizeof(buffer), 0);
    if (iResult > 0) {
        buffer[iResult] = '\0';
        username = std::string(buffer);
        Logger::getInstance().log("Client connected with username: " + username);
    } else {
        Logger::getInstance().log("Failed to receive username from client.");
        return; // Exit the method if username is not received
    }

    while (running) {
        iResult = recv(client_sock, buffer, sizeof(buffer), 0);
        if (iResult > 0) {
            buffer[iResult] = '\0';
            std::string message(buffer);
            Logger::getInstance().log("Received from client " + username + ": " + message);

            if (message[0] == '/') {
                processCommand(message);
            } else {
                // Forward message to all clients
                server->broadcastMessage(username + ": " + message);
            }
        } else if (iResult == 0) {
            Logger::getInstance().log("Client " + username + " disconnected.");
            break;
        } else {
            Logger::getInstance().log("recv failed from client " + username + ": " + strerror(errno));
            break;
        }
    }

    close(client_sock);
    server->removeClientHandler(this); // Notify the server to remove this client handler
}

void ClientHandler::processCommand(const std::string& command) {
    // Exemplu de procesare a comenzilor
    if (command == "/exit") {
        Logger::getInstance().log("Client " + username + " requested to exit.");
        stop();
        close(client_sock);
    } else {
        Logger::getInstance().log("Unknown command from client " + username + ": " + command);
    }
}

void ClientHandler::sendMessage(const std::string& message) {
    if (client_sock != -1) {
        send(client_sock, message.c_str(), message.size(), 0);
        Logger::getInstance().log("Sent to client " + username + ": " + message);
    }
}

std::string ClientHandler::getUsername()  {
    return username;
}
