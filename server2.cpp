

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

class chat_participant
{
public:
  virtual ~chat_participant() {}
  virtual void store(std::string msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

class chat_room
{
public:
  void join(chat_participant_ptr participant)
  {
    participants_.insert(participant);
    for (auto msg: recent_msgs_)
      participant->store(msg);
  }

  void leave(chat_participant_ptr participant)
  {
    participants_.erase(participant);
  }

  void deliver(std::string msg)
  {
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs)
      recent_msgs_.pop_front();

    for (auto participant: participants_)
      participant->store(msg);
  }

private:
  std::set<chat_participant_ptr> participants_;
  enum { max_recent_msgs = 100 };
  command_queue recent_msgs_;
};

//----------------------------------------------------------------------

class ss_session
  : public chat_participant,
    public std::enable_shared_from_this<ss_session>
{
public:
  ss_session(tcp::socket socket)
    : socket_(std::move(socket))
  {
  }

  void start()
  {
    //room_.join(shared_from_this());
    ID++;
    std::stringstream ss;
    ss << ID;
    std::string id = ss.str() + "\n";

    boost::asio::async_write(socket_, boost::asio::buffer(id), boost::bind(&ss_session::handle_write, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    do_read();
  }

  void store(std::string msg)
  {
    //Adds command to list and prints it
    commands.push_back(msg);
    std::cout << commands.front().data() << std::endl;
  }

private:
  void handle_write(const boost::system::error_code& e, std::size_t size){
  }
  void handler(const boost::system::error_code& e, std::size_t size)
  {
    std::cout << "reading sheet" << std::endl;
    if (!e)
      {
	std::istream is(&b);
	std::getline(is, read_msg_);
	store(read_msg_);
	if(b.size() > 0){
	  do_read();
	}
      }
    else{
      //room_.leave(shared_from_this());
    }
  }
  void do_read()
  {  
    auto self(shared_from_this());
    std::cout << "reading sheet" << std::endl;
    boost::asio::async_read_until(socket_, b,'\n', boost::bind(&ss_session::handler,shared_from_this(),boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
    
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
	    std::cout << "Client connected" << std::endl;
            std::make_shared<ss_session>(std::move(socket_))->start();
	  }	
	  //recursively loop for life of server
          do_accept();
        });
  }

  tcp::acceptor acceptor_;
  tcp::socket socket_;
};

//----------------------------------------------------------------------

int main(int argc, char* argv[])
{
  try
  {
    //start io_service
    boost::asio::io_service io_service;

    //list of ss_servers (this application can handle multiple separate servers for simultaneously  on different ports by adding more servers to list)
    std::list<ss_server> servers;
    
    //setting default ip and port
    tcp::endpoint endpoint(tcp::v4(), 2112);

    //creating new ss_server
    servers.emplace_back(io_service, endpoint);
    
    //running server's io
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
