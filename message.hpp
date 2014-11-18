#ifndef NETLAB_MSG
#define NETLAB_MSG

#include "common.hpp"

class message
{
public:
  message(std::string m, std::string i) {
    this->msg = m;
    this->ip = i;
  }
  ~message(){}

  inline std::string getIp() { return this->ip; }
  inline std::string getMsg() { return this->msg; }

private:
  std::string msg;
  std::string ip;

  
};

#endif