CFLAGS= -std=c++11 -Wall -I . -I .. -I ../../build/release-install-cpp11/include
LFLAGS= -L../../build/release-install-cpp11/lib -ljsoncpp -lmuduo_net -lmuduo_base -pthread

all: test_netserver test_netclient test_msg

test_netserver: test_net_server.cpp
	g++ -g $(CFLAGS) $^ -o $@ $(LFLAGS)

test_netclient: test_net_client.cpp
	g++ -g $(CFLAGS) $^ -o $@ $(LFLAGS)

test_msg: test_msg.cpp
	g++ -g $(CFLAGS) $^ -o $@ $(LFLAGS)

clean:
	rm -f test_netserver test_netclient test_msg 