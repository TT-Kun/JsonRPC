CFLAG= -std=c++11 -I ../../build/release-install-cpp11/include/
LFLAG= -L../../build/release-install-cpp11/lib/ -lmuduo_net -lmuduo_base  -pthread
all: dict_server dict_client
dict_server: dict_server.cpp
	g++ $(CFLAG) $^ -o $@ $(LFLAG)
dict_client: dict_client.cpp
	g++ $(CFLAG) $^ -o $@ $(LFLAG)
clean:
	rm dict_server
	rm dict_client
