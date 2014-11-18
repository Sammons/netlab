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
      FD_SET(new_sock, &set);
      this->max_fd = std::max(this->max_fd, new_sock);
      std::cout << "connection recieved on fd : " << new_sock << std::endl;
      this->add_conn(new_sock, client_address, socket_length);
    }
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    int time_to_do_something = select(
      this->max_fd+1,
      &set,
      NULL, NULL, &tv
      );
    if (time_to_do_something > 0) 
    {
      for (auto conn : this->client_socks) 
      {
        if (FD_ISSET(conn.sock_desc, &set)) 
        {
          char str[1024] = {'0'};
          int recv_result = recv(conn.sock_desc, str, 1023, MSG_DONTWAIT);
          std::cout << std::string(str)<<std::endl;
          if (recv_result == 0) {
            std::cout << "connection closed" << std::endl;
          }
        }
      }
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
  this->client_socks.push_back(new_con);
}

server::~server() 
{

}

#endif