#ifndef __COMMON__H
#define __COMMON__H

#include <string>

#include "Socket.h"

constexpr int TIMEOUT = 100;

bool bind_socket(const Socket& _socket,
					const std::string& _iface_addr,
                    const int _iface_port);
bool listen_socket(const Socket& _socket);
bool accept_socket(const Socket& _listenSocket, Socket& _dataSocket, std::string& _peerAddr);
bool read_socket(const Socket& _sock, std::string& _msg);
void connect_host(const Socket& _socket, const std::string& _host, const int _port );

extern bool g_isLoop;

void DefineSignals();

#endif