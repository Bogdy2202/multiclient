#include "Server.h"
#include "ClientHandler.h"
#include <iostream>
#include <cstring> // pentru memset
#include <arpa/inet.h>

Server::Server(int port) : listen_sock(-1), running(false) {
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == -1) {
        std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    memset(&saServer, 0, sizeof(saServer));
    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = INADDR_ANY;
    saServer.sin_port = htons(port);

    if (bind(listen_sock, (sockaddr*)&saServer, sizeof(saServer)) == -1) {
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
}

void Server::acceptClients() {
    while (running) {
        sockaddr_in coming_addr;
        socklen_t size = sizeof(coming_addr);
        int client_sock = accept(listen_sock, (sockaddr*)&coming_addr, &size);
        if (client_sock == -1) {
            std::cerr << "Accept failed: " << strerror(errno) << std::endl;
            continue;
        }

        char buffer[36];
        inet_ntop(AF_INET, &coming_addr.sin_addr, buffer, sizeof(buffer));
        std::cout << buffer << " connected!\n";

        auto handler = std::make_unique<ClientHandler>(client_sock, this);
        clientHandlers.push_back(std::move(handler));
    }
}

void Server::broadcastMessage(const std::string& message, int sender_sock) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (auto& handler : clientHandlers) {
        if (handler->getClientSock() != sender_sock) {
            handler->sendMessage(message);
        }
    }
}
