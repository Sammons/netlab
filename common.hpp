
#ifndef NETLAB_HEADERS
#define NETLAB_HEADERS

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>

typedef struct sockaddr_in socket_address_in;
typedef struct sockaddr socket_address;
#include "helpers.hpp"
#include "input_queue.hpp"
#include "output_queue.hpp"
#include "message.hpp"
#include "server.hpp"
#include "client.hpp"


#endif