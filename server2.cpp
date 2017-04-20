/**
 * Server Spreadsheet
 * Author: MemeTeam
 * Date: 4/20/2017
 * Asynchronous
 * "Meme Up"
 */

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include <boost/bind.hpp>


using boost::asio::ip::tcp;
static int ID = 0;

//----------------------------------------------------------------------

//holds commands of all clients
typedef std::deque<std::string> command_queue;

//----------------------------------------------------------------------

class ss_user
{
public:
  virtual ~ss_user(){}
  virtual void send(std::string data) = 0;

  
  int userid;
  std::string username;
};

typedef std::shared_ptr<ss_user> client_ptr;

//----------------------------------------------------------------------

class chat_room

{
public:
  void join(client_ptr c, std::string name)
  {
    clients.insert(c);
    c->username = name;
  }

  void leave(client_ptr c)
  {
    clients.erase(c);
  }

  void send(int id, std::string data)
  {
    for(auto client: clients){
      if(client->userid == id){
	client->send(data);
      }
    }
  }

private:
  std::set<client_ptr> clients;
  enum { max_recent_msgs = 100 };
  command_queue recent_msgs_;
};

//----------------------------------------------------------------------

class ss_session
  : public ss_user,
    public std::enable_shared_from_this<ss_session>
{
public:
  ss_session(tcp::socket socket)
    : socket_(std::move(socket))
  {
  }

  void start()
  {
    int newid = ID++;
    std::stringstream ss;
    ss << newid;

    std::string id = ss.str() + "\n";
    shared_from_this()->userid = newid;
      
    boost::asio::async_write(socket_, boost::asio::buffer(id), boost::bind(&ss_session::handle_write, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    read_login();
    
  }
  
  void send(std::string data){
    
    boost::asio::async_write(socket_, boost::asio::buffer(data), boost::bind(&ss_session::handle_write, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
  }
  
  
  
  //Stores commands from server in commands queue
  void store(std::string msg)
  {
    //Adds command to list and prints it
    commands.push_back(msg);
    std::cout << commands.front().data() << std::endl;
  }

private:
  void handle_write(const boost::system::error_code& e, std::size_t size){
  }
  
 
  
  void read_login()
  {  
    auto self(shared_from_this());
    std::cout << "attempting login..." << std::endl;
    boost::asio::async_read_until(socket_, b,'\n', boost::bind(&ss_session::handle_login,shared_from_this(),boost::asio::placeholders::error,
							       boost::asio::placeholders::bytes_transferred));    
  }

 void handle_login(const boost::system::error_code& e, std::size_t size)
  {
    if (!e)
      {
	std::istream is(&b);
	std::getline(is, read_msg_);
	std::cout << "Username:\n" << read_msg_ << "\nhas successfully connected." << std::endl;
	room_.join(shared_from_this(), read_msg_);
	read();
	
      }
    else{
      //room_.leave(shared_from_this());
    }
  }


  void read()
  {  
    auto self(shared_from_this());
    std::cout << "reading..." << std::endl;
    boost::asio::async_read_until(socket_, b,'\n', boost::bind(&ss_session::handle_read,shared_from_this(),boost::asio::placeholders::error,
							       boost::asio::placeholders::bytes_transferred));
  }

  void handle_read(const boost::system::error_code& e, std::size_t size)
  {
    if (!e)
      {
	std::istream is(&b);
	std::getline(is, read_msg_);
	store(read_msg_);
	send("Message recieved");
	read();
	
      }
    else{
      //room_.leave(shared_from_this());
    }
  }

  void do_write()
  {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(commands.front().data(), commands.front().length()),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            commands.pop_front();
            if (!commands.empty())
            {
              do_write();
            }
          }
          else
          {
            //room_.leave(shared_from_this());
          }
        });
  }
  
  boost::asio::streambuf b;
  tcp::socket socket_;
  std::string read_msg_;
  command_queue commands;
  chat_room room_;
};

//----------------------------------------------------------------------

class ss_server
{
public:
  ss_server(boost::asio::io_service& io_service,
      const tcp::endpoint& endpoint)
    : acceptor_(io_service, endpoint),
      socket_(io_service)
  {
    //accept clients
    do_accept();
  }

private:
  void do_accept()
  {
    acceptor_.async_accept(socket_,
        [this](boost::system::error_code ec)
        {
          if (!ec)
          {
	    //std::cout << "socket accepted" << std::endl;
            std::make_shared<ss_session>(std::move(socket_))->start();
	  }	
	  //recursively loop for life of server
          do_accept();
        });
  }

  tcp::acceptor acceptor_;
  tcp::socket socket_;
};

typedef boost::shared_ptr<ss_server> ss_server_ptr;
//----------------------------------------------------------------------

int main(int argc, char* argv[])
{
  try
  {
    //start io_service
    boost::asio::io_service io_service;    
    
    //setting default ip and port
    tcp::endpoint endpoint(tcp::v4(), 2112);

    //creating new ss_server
    ss_server_ptr server(new ss_server(io_service, endpoint));
    
    
    std::cout << "Server online, fck u Filip" << std::endl;

    //running server's io
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
