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
                Logger::getInstance().log("Client " + username + " requested command.");
                processCommand(message);
            }
            else {
                Logger::getInstance().log("Received from client " + username + ": " + message);
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
    if (command == "/exit")
    {
        Logger::getInstance().log("Client " + username + " requested to exit.");
        this->sendMessage("EXIT");
        stop();
        //server->removeClientHandler(this);
    }
    else if (command.substr(0, 9) == "/message_") {
        size_t space_pos = command.find(' ', 9);
        if (space_pos != std::string::npos)
        {
            std::string target_username = command.substr(9, space_pos - 9);
            std::string private_message = command.substr(space_pos + 1);
            Logger::getInstance().log("Private message from " + username + " to " + target_username + ": " + private_message);
            server->sendPrivateMessage(target_username, "Private message from " + username + ": " + private_message);
        }
    }
        else if (command == "/show_users") {
            std::vector<std::string> users = server->getUsernames();
            std::string user_list = "Connected users:\n";
            for (const auto& user : users) {
                user_list += user + "\n";
            }
            sendMessage(user_list);
        }
     else {
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
