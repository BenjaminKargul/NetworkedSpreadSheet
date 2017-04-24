all:
	g++ -std=c++11 server2.cpp /usr/local/lib/libboost_system.a /usr/local/lib/libboost_regex.a -o server


clean:
	rm server
