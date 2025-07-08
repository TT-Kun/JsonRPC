CFLAGS= -std=c++11 -Wall -I . -I ../.. -I ../../../build/release-install-cpp11/include
LFLAGS= -L../../../build/release-install-cpp11/lib -ljsoncpp -lmuduo_net -lmuduo_base -pthread

all: test_rpc_server test_rpc_client

test_rpc_server: test_rpc_server.cpp
	g++ -g $(CFLAGS) $^ -o $@ $(LFLAGS)

test_rpc_client: test_rpc_client.cpp
	g++ -g $(CFLAGS) $^ -o $@ $(LFLAGS)

clean:
	rm -f test_rpc_server test_rpc_client 