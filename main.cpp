#include "common.hpp"
#include "server.cpp"
#include "client.cpp"
#define MAXCLIENTS 3

int main(int argc, char const *argv[])
{
  std::vector<std::string> arguments = helpers::get_args(argc, argv);

  if (helpers::arguments_are_invalid(arguments)) 
  {
    helpers::print_usage();
    return 1;
  }

  auto mode = arguments[1];
  if (mode == "server") 
  {
    std::cout 
      << "starting server at port : " 
      << helpers::settings["port"]
      << std::endl;
      try {
        server the_only_server = server( "16590",  MAXCLIENTS );
        the_only_server.start();
      } catch( int e ) {
        helpers::error_msg(e);
        return 1;
      }
  }
  else if (mode == "client") 
  {
    std::cout
    << "client started"
    << std::endl;
    try {
      client one_of_many_clients = client( "127.0.0.1", "16590" );
    } catch ( int e ) {
      helpers::error_msg(e);
      return 1;
    }
  }

  return 0;
}