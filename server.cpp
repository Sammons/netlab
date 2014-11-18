#ifndef NETLAB_SERVER_CPP
#define NETLAB_SERVER_CPP

#include "common.hpp"
#include "server.hpp"



server::server( std::string port, int maxclients )
{
  this->maxclients = maxclients;
  this->max_fd = 0;
  this->socket_file_desc = helpers::init_tcp_socket();
  if (this->socket_file_desc < 0) 
  {
    throw NETLAB_FAILED_TO_INIT_SOCKET;
  }
  std::cout << "initialized fd : " << this->socket_file_desc << std::endl;
  this->server_address = helpers::init_address(
    "127.0.0.1", port 
    );

  if ( 
    bind( 
      this->socket_file_desc, 
      (const struct sockaddr*)&(this->server_address),
      sizeof(this->server_address)
      ) < 0)
  {
    std::cout << "C Error number: "<< errno << std::endl;
    throw NETLAB_FAILED_TO_BIND_SOCKET;
  }
}

void server::start() 
{
  fd_set set;
  FD_ZERO(&set);
  while(true) 
  {
    /* willing to queue twice as many clients as
    we will plan to handle */
    listen(this->socket_file_desc, 2*this->maxclients);

    socket_address client_address;
    socklen_t socket_length;
    struct timeval tv;
    int new_sock = accept4(this->socket_file_desc,
      &client_address,
      &socket_length,
      SOCK_NONBLOCK);

    if (new_sock > 0) {
      std::cout << "connection recieved on fd : " << new_sock << std::endl;
      this->add_conn(new_sock, client_address, socket_length);
    }
    for (int i = 0; i < this->client_socks.size(); i++) 
    {
      char str[1025];
      auto conn = client_socks[i];
      int recv_result = recv(conn.sock_desc, str, 1024, MSG_DONTWAIT);
      if (recv_result == 0 && conn.closed == false) {
        client_socks[i].closed = true;
        std::cout << "connection closed on fd : " << conn.sock_desc << std::endl;
      }
      else if (recv_result > 0) {
        std::string cur_str = std::string(str);
        client_socks[i].incoming.push(cur_str);
        //log: std::cout << "message recieved of length : " << cur_str.size() << std::endl;
      }
    }
    /* process messages */
    for (int i = 0; i < this->client_socks.size(); i++)
    {
      try 
      {
        std::string tmp_str = "";
        std::string tmp_raw_str = "";
        std::vector<std::string> raw_messages;
        while ( !this->client_socks[i].incoming.empty() 
        && tmp_str.size() < sizeof(char )*100000/* 1/10 mb */ )
        {
          tmp_str += this->client_socks[i].incoming.back();
          this->client_socks[i].incoming.pop();
        }

        long start = tmp_str.find("::b::");
        long dbl_start = 0;
        long end = tmp_str.find("::e::");
        while (start != tmp_str.npos 
          && end != tmp_str.npos
          && tmp_str.size() != 0) 
        {
        

          if (start > end) 
          {
            //log: std::cout << "skip junk" << std::endl;
            tmp_str = tmp_str.substr(start);
            start = tmp_str.find("::b::");
            end = tmp_str.find("::e::");
            continue;
          }
 
          if (start != 0)
          {
            tmp_str.substr(0, start);
            start = tmp_str.find("::b::");
            end = tmp_str.find("::e::");
            continue;
          }

          dbl_start = tmp_str.find("::b::", start + 1);
          if (dbl_start < end)
          {
            start = dbl_start;
          }


          start += 5;

          std::string msg = tmp_str.substr(start, end-start);

          //log: std::cout << "message :" << msg << std::endl;
          client_socks[i].push(msg);
          tmp_str = tmp_str.substr( start + (end-start) );

          start = tmp_str.find("::b::");
          end = tmp_str.find("::e::");
        }
      } 
      /* the show must go on */
      catch (...) 
      {
        helpers::error_msg(1);
      }

      /* process commands for each client */
    }
  }
}

void server::add_conn(
  int sock_desc,
  socket_address addr,
  socklen_t len ) 
{
  connection_t new_con = connection_t(
    sock_desc, addr, len
    );
  new_con.closed = false;
  this->client_socks.push_back(new_con);
}

server::~server() 
{
  close(socket_file_desc);
  for (auto conn : this->client_socks) 
  {
    close(conn.sock_desc);
    conn.closed = true;
  }
}

#endif