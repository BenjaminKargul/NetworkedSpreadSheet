all:
	g++ -std=c++11 Server.cpp /usr/local/lib/libboost_system.a -o server
	g++ -std=c++11 Client.cpp /usr/local/lib/libboost_system.a -lpthread -o client
