#include <iostream>

#if defined (CLIENT)
#include "client/client.hpp"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "example: ./client <name> <port> <period>\n";
        return -1;
    }

    Client client;

    client.Handle(argv);

    return 0;
}
#elif defined (SERVER)
#include "server/server.hpp"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "example: ./server <port>\n";
        return -1;
    }

    Server server;

    server.Handle(argv[1]);

    return 0;
}
#endif
