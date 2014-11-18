#ifndef NETLAB_HELPERS
#define NETLAB_HELPERS
#include "common.hpp"

namespace helpers {

  std::string get_arg(const int argc, const char* argv[], int index) 
  {
    if (index >= argc) return std::string("");
    else if (index < 0) return std::string("");
    else 
    {
      return std::string(argv[index]);
    }
  }

  std::vector<std::string> get_args(const int argc, const char* argv[]) 
  {
    std::vector<std::string> arguments_vector;
    for (int i = 0; i < argc; i++) 
    {
      arguments_vector.push_back( get_arg(argc, argv, i) );
    }
    return arguments_vector;
  }

  inline int init_tcp_socket() 
  {
    /* internet, tcp, & init to 0 */
    int tcp_socket = socket(AF_INET, SOCK_STREAM | O_NONBLOCK, 0);

    /* let folks later deal */
    return tcp_socket;
  }

  socket_address_in init_address( std::string ip, std::string port ) 
  {
    socket_address_in address = {0};

    /* ... addresses from the internet */
    address.sin_family = AF_INET;

    /* sets the sin_address, aka sets up
    local interfaces */
    inet_pton(AF_INET, ip.c_str(), &address.sin_addr);

    /* port to listen on */
    address.sin_port = htons( std::stoi( port ) );

    return address;
  }

  inline void print_usage() {
    std::cout << std::endl;
    std::cout << "Usage of this chat program:"                      << std::endl;
    std::cout << "\t./chat server (begin server)"                   << std::endl;
    std::cout << "\t./chat client (try to attach client to server)" << std::endl;
    std::cout << "NOTE: defaults are 127.0.0.1 @ port 16590"        << std::endl;
    std::cout << "--Program by Ben Sammons, 2014--"                 << std::endl<<std::endl;
  }

  bool arguments_are_invalid(std::vector<std::string> &arguments) 
  {
    int num_args = static_cast<int>(arguments.size());
    if ( num_args < 2 || num_args > 2)
    {
      std::cout << std::endl;
      std::cout << "expected # of arguments: 2" <<std::endl;
      std::cout << "received : " << num_args << std::endl;
      return true;
    } 

    const std::unordered_set<std::string> valid_second_arguments = {
      "server",
      "client"
    };

    if ( valid_second_arguments.find( arguments[1] ) 
        == valid_second_arguments.end() )
    {
      std::cout << std::endl;
      std::cout << "argument invalid : " << arguments[1] << std::endl;
      return true;
    }

    return false;
  }

  std::unordered_map<std::string, std::string> settings = {
    { "port", "16590"    },
    { "ip"  , "127.0.0.1"}
  }; 

  #define NETLAB_GENERIC_FAILURE 1
  #define NETLAB_FAILED_TO_BIND_SOCKET 2
  #define NETLAB_FAILED_TO_INIT_SOCKET 3
  std::unordered_map<int, std::string> errors = {
    { NETLAB_GENERIC_FAILURE, "generic failure" },
    { NETLAB_FAILED_TO_BIND_SOCKET, "failed to bind socket"},
    { NETLAB_FAILED_TO_INIT_SOCKET, "failed to init socket"}
  };

  void error_msg(int e) {
    if (errors.find(e) == errors.end()) {
      std::cout << "unrecognized failure" << std::endl;
      return;
    }
    std::cout << errors[e] << std::endl;
    return;
  }
    
}

#endif