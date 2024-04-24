#ifndef PBF_GROUP_CLIENT_CLIENT_HPP
#define PBF_GROUP_CLIENT_CLIENT_HPP

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

#endif // PBF_GROUP_CLIENT_CLIENT_HPP
