/**
 * Server Spreadsheet
 * Author: MemeTeam
 * Date: 4/20/2017
 * Asynchronous Server based on Uinta protocol
 */

#include <cstdlib>
#include <deque>
#include <iostream>
#include <fstream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <dirent.h>
#include <map>
#include <boost/regex.hpp>
#include "dg.cpp"
#include "ss.cpp"

using boost::asio::ip::tcp;
//client identification number
int ID = 0;

//Map of ID's to Documents
std::map<int,std::string> sheetkey;
std::map<std::string,int> sheetvalue;

//Value to hold the next created or read documents id
int docID= 0;


std::map<int,Spreadsheet*> open_sheets;

boost::regex r("[a-zA-Z0-9_-]+");
//----------------------------------------------------------------------

//holds commands of all clients
typedef std::deque<std::string> command_queue;

//----------------------------------------------------------------------

class ss_user
{
public:
  virtual ~ss_user(){}
  virtual void send(std::string data) = 0;

  std::set<int> opensheet;
  int userid;
  std::string username;
};

typedef std::shared_ptr<ss_user> client_ptr;

//----------------------------------------------------------------------

class lobby

{
public:
  lobby(){}
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
      if(client->opensheet.count(sheetid) > 0){
	client->send(data);
      }
    }
    
  }
  //Send to all users
  void sendAll(std::string data){
    std::cout << "sending to all" << std::endl;
    for(auto client: clients){
      std::cout << client->username <<std::endl;
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
static lobby room_;
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
      break;
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
      std::cout<<"Sending Valid Undo"<<std::endl;      
      undo(contents);
      break;
    case 5: //Redo
      std::cout<<"Sending Valid Redo"<<std::endl;      
      redo(contents);
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
    std::string documentID = contents.substr(0,contents.find('\t'));
    int dID = stoi(documentID);
    std::string filename = contents.substr(contents.find('\t')+1);

    //If a document already exists with the filename
    if(sheetvalue.count(filename) > 0){
      std::cout << "Filename already exists" << std::endl;

      send("9\t" + documentID + "\n");
      return;
    }
    //If the filename requested has a '/' character (cannot change directory
    if(!regex_match(filename,r)){
      std::cout << "Innapropriate rename" << std::endl;
      send("9\t" + documentID + "\n");
      return;
    }

    std::string name1 ="spreadsheets/" + sheetkey[dID];
    std::string name2 ="spreadsheets/" + filename;
    // std::cout<<name1<<std::endl;
    //std::cout<<name2<<std::endl;
    int result = std::rename( name1.c_str(), name2.c_str() );
    //std::cout<<result<<std::endl;
    if(result == 0){
      
      //remove entry in sheetvalue
      sheetvalue.erase(sheetkey[dID]);
      //change sheetkey entry
      sheetkey[dID] = filename;
      //add new entry to sheetvalue
      sheetvalue[filename] = dID;

      send("8\t" + documentID + "\n");
      room_.sendAll("6\t" + contents + "\n");
      
    }
    else{
      send("9\t" + documentID + "\n");
    }
  }

  void edit(){
    //checks for commands, facilitates edit, then calls itself is more commands are queued up
    if(commands.size() > 0){
      handle_edit(commands.front().data());
      std::string content = commands.front().data();
      std::string documentID = content.substr(0,content.find('\t'));
      int dID = stoi(documentID);
      std::stringstream ss;
      ss << dID;
      
      std::string msg = "4\t" + ss.str() + "\n";
      send(msg);
      
      
      commands.pop_front();
    
      
      
      if(commands.size()>0){
	edit();
      }
    }
  }

  
  int handle_edit(std::string command){  
    std::string documentID = command.substr(0,command.find('\t'));
    int dID = stoi(documentID);
    std::string cellinfo = command.substr(command.find('\t')+1);
    std::string cellname = cellinfo.substr(0,cellinfo.find('\t'));
    std::string cellcontents = cellinfo.substr(cellinfo.find('\t')+1);
    
    open_sheets[dID]->SetCellContents(cellname,cellcontents);
    std::stringstream ss;
    ss << dID;
    
    std::string msg = "3\t" + ss.str() +"\t"+ cellname +"\t"+ cellcontents + "\n";
    room_.sendAll(msg);
    
    return 0;
  }
  
  void openNew(std::string contents){
    //If a document already exists with the filename
    if(sheetvalue.count(contents) > 0){
      fileList();
      return;
    }
    //If the filename requested has a '/' character (cannot change directory
    if(!regex_match(contents,r)){
      fileList();
      return;
    }
    std::stringstream ss;
    ss << docID;
    
    std::string msg = "1\t" + ss.str() + "\n";
    std::string filename = "spreadsheets/" + contents;
    std::ofstream {filename};
    send(msg);
    
    sheetkey[docID] = contents;
    sheetvalue[contents] = docID;
    //open sheet
    open_sheets[docID] = new Spreadsheet();
    //add to users open list
    shared_from_this()->opensheet.insert(docID);
    
    
    docID++;
  }
  
  void open(std::string contents){ 
    
    int dID = sheetvalue[contents];
    if(open_sheets.count(dID) == 0){
      open_sheets[dID] = new Spreadsheet();
    }
    shared_from_this()->opensheet.insert(docID);
    std::stringstream ss;
    ss << dID;

    std::string msg = "2\t" + ss.str() + "\n";

    send(msg);
  }
  
  void save(std::string contents){
    //shared_from_this()->opensheet
    int dID = stoi(contents);
    std::string filename = "spreadsheets/" + sheetkey[dID];
    
    //Names of cells to loop through and save
    std::list<std::string> x = open_sheets[dID]->GetNamesOfAllNonemptyCells();
    
    send("7\t"+ contents + "\n");    
  }
  void undo(std::string contents){
    send("4\t" + contents + "\n");
  }  
  void redo(std::string contents){
    send("4\t" + contents + "\n");
  }
  
  void fileList(){
    std::cout << "file list requested" << std::endl;
    std::string files = "0";
    for(int i = 0; i < sheetkey.size(); i++){
      files += "\t" + sheetkey[i];
    }
    files += "\n";
    send(files);
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
      room_.leave(shared_from_this());
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
    std::cout << "######################################\n#   SPREADSHEET SERVER BY MEMETEAM   #\n######################################" << std::endl;

    std::cout << "Initializing..." << std::endl;


    std::string ssname = "";
    DIR *dp;
    struct dirent *dirp;
    std::string dir = "spreadsheets/";
    dp = opendir( dir.c_str() );
    if (dp != NULL){
      
      std::cout << "Files avaliable:" << std::endl;
      //Iterate through directory
      while ((dirp = readdir( dp ))){
	
	//Read file name
	ssname = dirp->d_name;
	
	//Ignore files "." and ".."
	if( !std::strcmp( ssname.c_str(), "." ) == 0 && !std::strcmp( ssname.c_str(), "..") == 0){
	  sheetkey[docID] = ssname;
	  sheetvalue[ssname] = docID;
	  docID++;
	  std::cout << ssname << ", ";
	}
      }
      std::cout << std::endl;
      
    }
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
