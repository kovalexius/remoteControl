#include "Socket.h"

#include <iostream>
#include <set>

int main()
{
    Socket sock1;
    Socket sock2(sock1);
    Socket sock3;
    Socket sock4;
    Socket sock5;
    Socket sock6(std::move(sock1));

	std::set<Socket> sockList;
	sockList.insert(sock3);
    sockList.insert(sock4);
    sockList.erase(sock3);

	std::cin.get();
}