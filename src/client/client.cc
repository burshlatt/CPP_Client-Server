#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "client.hpp"

std::string Client::GetCurrentTimestamp() {
    auto now{std::chrono::system_clock::now()};
    auto now_time_t{std::chrono::system_clock::to_time_t(now)};
    auto ms{std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch())};
    auto now_tm{*std::localtime(&now_time_t)};
    char time_str[100];

    std::strftime(time_str, sizeof(time_str), "[%Y-%m-%d %H:%M:%S", &now_tm);
    std::sprintf(time_str + std::strlen(time_str), ".%03ld]", ms.count() % 1000);

    return time_str;
}

void Client::Handle(char* argv[]) {
    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(std::stoi(argv[2]));
    inet_pton(AF_INET, "127.0.0.1", &s_addr.sin_addr);

    while (true) {
        int c_socket{socket(AF_INET, SOCK_STREAM, 0)};

        if (c_socket == -1)
            throw std::runtime_error("socket()");

        if (connect(c_socket, (struct sockaddr*)&s_addr, sizeof(s_addr)) < 0)
            throw std::runtime_error("connect()");

        std::string msg{std::move(GetCurrentTimestamp() + " " + argv[1])};

        send(c_socket, msg.data(), msg.size(), 0);
        close(c_socket);

        std::this_thread::sleep_for(std::chrono::seconds(std::stoi(argv[3])));
    }
}
