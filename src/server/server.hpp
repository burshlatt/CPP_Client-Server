#ifndef CLIENT_SERVER_SERVER_SERVER_HPP
#define CLIENT_SERVER_SERVER_SERVER_HPP

#include <string_view>

class Server {
public:
    Server() = default;
    ~Server() = default;

public:
    void Handle(const std::string& port);

private:
    void SaveLogs(std::string_view data);
    void HandleClient(int client_socket);
};

#endif // CLIENT_SERVER_SERVER_SERVER_HPP
