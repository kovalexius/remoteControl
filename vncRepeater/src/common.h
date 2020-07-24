#ifndef __COMMON__H
#define __COMMON__H

#include <string>

#include "Socket.h"

void bind_socket(const Socket& _socket,
					const std::string& _iface_addr,
                    const int _iface_port);

void listen_socket(const Socket& _socket);

#endif