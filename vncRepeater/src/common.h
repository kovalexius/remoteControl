#ifndef __COMMON__H
#define __COMMON__H

#include <string>

#include "Socket.h"

bool bind_socket(const Socket& _socket,
					const std::string& _iface_addr,
                    const int _iface_port);

bool listen_socket(const Socket& _socket);

bool accept_socket(const Socket& _listenSocket, Socket& _dataSocket, std::string& _peerAddr);

#endif