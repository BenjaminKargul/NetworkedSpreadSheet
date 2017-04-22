all:
	g++ -std=c++11 server2.cpp /usr/local/lib/libboost_system.a -o server


clean:
	rm server
