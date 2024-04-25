#ifndef CLIENT_SERVER_CLIENT_CLIENT_HPP
#define CLIENT_SERVER_CLIENT_CLIENT_HPP

#include <string>

class Client {
public:
    Client() = default;
    ~Client() = default;

public:
    void Handle(char* argv[]);

private:
    std::string GetCurrentTimestamp();
};

#endif // CLIENT_SERVER_CLIENT_CLIENT_HPP
