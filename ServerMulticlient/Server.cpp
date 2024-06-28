#include "Server.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include "Logger.h"
#include<algorithm>

Server::Server(int port) : listen_sock(-1), running(false) {
    Logger::getInstance().log("Server starting...");

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == -1) {
        std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = INADDR_ANY;
    saServer.sin_port = htons(port);

    if (bind(listen_sock, (struct sockaddr*)&saServer, sizeof(saServer)) == -1) {
        std::cerr << "Bind failed: " << strerror(errno) << std::endl;
        close(listen_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(listen_sock, SOMAXCONN) == -1) {
        std::cerr << "Listen failed: " << strerror(errno) << std::endl;
        close(listen_sock);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << port << "...\n";
    Logger::getInstance().log("Server listening on port " + std::to_string(port));
}

Server::~Server() {
    stop();
}

void Server::start() {
    running = true;
    acceptClients();
}

void Server::stop() {
    running = false;
    close(listen_sock);
    for (auto& handler : clientHandlers) {
        handler->stop();
    }
    clientHandlers.clear();
    Logger::getInstance().log("Server stopped.");
}

void Server::acceptClients() {
    while (running) {
        struct sockaddr_in coming_addr;
        socklen_t size = sizeof(coming_addr);
        int client_sock = accept(listen_sock, (struct sockaddr*)&coming_addr, &size);
        if (client_sock == -1) {
            std::cerr << "Accept failed: " << strerror(errno) << std::endl;
            Logger::getInstance().log("Accept failed: " + std::string(strerror(errno)));
            continue;
        }
        else{
            char buffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &coming_addr.sin_addr, buffer, sizeof(buffer));
            Logger::getInstance().log(std::string(buffer) + " connected!");
            std::cout << buffer << " connected!\n";

            auto handler = std::make_unique<ClientHandler>(client_sock, this);
            clientHandlers.push_back(std::move(handler));

            // Start handling the client in a new thread
            clientHandlers.back()->start();
        }
    }
}

void Server::broadcastMessage(const std::string& message) {
    for (auto& handler : clientHandlers) {
        handler->sendMessage(message);
    }
}

void Server::sendPrivateMessage(const std::string& username, const std::string& message) {
    for (auto& handler : clientHandlers) {
        if (handler->getUsername() == username) {
            handler->sendMessage(message);
            break;
        }
    }
}

void Server::removeClientHandler(ClientHandler* handler) {
    auto it = std::remove_if(clientHandlers.begin(), clientHandlers.end(),
                             [handler](const std::unique_ptr<ClientHandler>& ptr) { return ptr.get() == handler; });

    if (it != clientHandlers.end()) {
        // Release the unique_ptr ownership without deleting the object
        handler = it->release();
        // Remove the now-nullptr from the vector
        clientHandlers.erase(it);
    }
}

std::vector<std::string> Server::getUsernames() const {
    std::vector<std::string> usernames;
    for (const auto& handler : clientHandlers) {
        usernames.push_back(handler->getUsername());
    }
    return usernames;
}

// void Server::removeClientHandler(ClientHandler* handler) {
//     auto it = std::remove_if(clientHandlers.begin(), clientHandlers.end(),
//                              [handler](const std::unique_ptr<ClientHandler>& ptr) { return ptr.get() == handler; });
//
//     if (it != clientHandlers.end()) {
//         clientHandlers.erase(it, clientHandlers.end());
//     }
// }

