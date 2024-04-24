#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "server.hpp"

void Server::SaveLogs(std::string_view data) {
    std::ofstream file("log.txt", std::ios::app);
    
    file << data << '\n';
}

void Server::HandleClient(int client_socket) {
    char buffer[1024]{0};

    if (recv(client_socket, buffer, 1024, 0) < 0) {
        std::cerr << "recv()\n";
        close(client_socket);
        return;
    }

    SaveLogs(buffer);
    close(client_socket);
}

void Server::Handle(const std::string& port) {
    int s_socket{socket(AF_INET, SOCK_STREAM, 0)};

    if (s_socket == -1)
        throw std::runtime_error("socket()");
    
    int opt{1};

    if (setsockopt(s_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
        throw std::runtime_error("setsockopt()");
    
    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = INADDR_ANY;
    s_addr.sin_port = htons(std::stoi(port));
    
    if (bind(s_socket, (struct sockaddr*)&s_addr, sizeof(s_addr)) < 0)
        throw std::runtime_error("bind()");
    
    while (true) {
        if (listen(s_socket, 10) < 0)
            throw std::runtime_error("listen()");
        
        std::cout << "Processing...\n";
        
        struct sockaddr_in c_addr;
        socklen_t c_addr_size{sizeof(c_addr)};
        int c_socket{accept(s_socket, (struct sockaddr*)&c_addr, &c_addr_size)};

        if (c_socket < 0)
            throw std::runtime_error("accept()");

        std::thread client_thread([this](int cs) {
            HandleClient(cs);
        }, c_socket);

        client_thread.detach();
    }
    
    close(s_socket);
}
