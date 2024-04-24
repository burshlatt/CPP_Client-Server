CXX          = g++
OPTIMIZATION = -O3
STANDARD     = -std=c++17
FLAGS        = -Wall -Werror -Wextra -pedantic

CLIENT = src/main.cc src/client/client.cc
SERVER = src/main.cc src/server/server.cc

all: clean
	$(CXX) $(FLAGS) $(OPTIMIZATION) $(STANDARD) $(CLIENT) -o client -DCLIENT
	$(CXX) $(FLAGS) $(OPTIMIZATION) $(STANDARD) $(SERVER) -o server -DSERVER

clean:
	rm -rf log.txt client server
	