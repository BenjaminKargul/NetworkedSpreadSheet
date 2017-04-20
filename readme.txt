Author: Meme_team
Date: 4/15/17


Server starts up, listens for, and accepts new client connections until the server is ended.

On recieving a connection it will send a single unique id (1 - MAX_INTEGER) and ends connection.

The next step is to make it stay connected so we can have multiple i/o interactions.



Client code connects and receives its unique id and prints. 

When making the c# client code you should be able to at least run tests with the unique id.



How to compile (type in shell):

make


these will run the following automatically through the makefile:

g++ -std=c++11 Client.cpp /usr/local/lib/libboost_system.a -lpthread -o client

g++ -std=c++11 Server.cpp /usr/local/lib/libboost_system.a -o server
