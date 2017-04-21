/**
 * Server Spreadsheet
 * Author: MemeTeam
 * Date: 4/20/2017
 * Asynchronous Server based on Uinta protocol
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

  int opensheet;
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
  //Send to members of the same open spreadsheet
  void sendGroup(int sheetid, std::string data){
    for(auto client: clients){
      if(client->opensheet == sheetid){
	client->send(data);
      }
    }
    
  }
  //Send to all users
  void sendAll(std::string data){
    for(auto client: clients){
      client->send(data);
    }
  }
  //private message
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
  
  
  
  //Handles commands from server in commands queue
  int handle_command(std::string msg)
  {
    
    std::string command = msg;
    
    std::string opcode = command.substr(0,command.find('\t'));
    int opc = stoi(opcode);
    std::string contents = command.substr(command.find('\t')+1);
    std::cout << "opcode " << opc << "\ncontents " << contents << std::endl;
    switch(opc){
    case 0:
      fileList();
    case 1: //New
      openNew(contents);
      break;
    case 2: //Open
      open(contents);
      break;
    case 3: //Edit
      //Adds command to list for undos
      commands.push_back(contents);
      
      edit();
      break;
    case 4: //Undo
      //undo();
      break;
    case 5: //Redo
      //redo();
      break;
    case 6:
      save(contents);
      break;
    case 7: //Rename
      rename(contents);
      break;
    }

    
  }
  
  void rename(std::string contents){
    send("6\tIDof" + contents);
  }

  void edit(){
    //checks for commands, facilitates edit, then calls itself is more commands are queued up
    if(commands.size() > 0){
      handle_edit(commands.front().data());
      commands.pop_front();
      
      if(commands.size()>0){
	edit();
      }
    }
    send("4\tIDofDocumentRequested\n");
  }
  
  void handle_edit(std::string command){    
  }
  
  void openNew(std::string contents){ 
    std::cout << "opening new sheet" << std::endl;
    send("1\t" + contents);
  }
  
  void open(std::string contents){   
    send("2\t" + contents);
  }
  
  void save(std::string contents){
    //shared_from_this()->opensheet
    send("7\t"+ contents);    
  }
  
  void fileList(){
    std::cout << "file list requested" << std::endl;
    send("0\tfile1.sprd\tfile2.sprd\n");
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
	handle_command(read_msg_);
	//send("Message recieved");
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
    
    
    std::cout << "Server online" << std::endl;

    //running server's io
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
