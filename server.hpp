#ifndef NETLAB_SERVER
#define NETLAB_SERVER

#include "common.hpp"

struct connection_t 
{
  int sock_desc;
  socket_address addr;
  socklen_t len;
  connection_t( int desc, socket_address addr,socklen_t len) {
    this->sock_desc = desc;
    this->addr = addr;
    this->len = len;
  }
};

class server
{
public:
  server( std::string port, int maxclients );
  ~server();

  void start();

private:
  int maxclients;
  std::queue<message> temp_q;
  output_queue out_q;
  input_queue in_q;
  std::vector<connection_t> client_socks;
  int max_fd;
  int socket_file_desc;
  
  struct sockaddr_in server_address;

  void add_conn(
    int sock_desc,
    socket_address addr,
    socklen_t len );

  bool input_queue_is_empty();
  void drain_input_queue();
  void drain_output_queue();

};

#endif