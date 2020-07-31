/*
relay(репитер) слушает два разных порта.
Когда на какой-либо порт приходит новое соединение, то репитер ожидает от него строчки вида ID:<какой-то идентификатор>
Остальные сообщения игнорируются (куда нибудь логируется как ошибка)
По приходу ожидаемой строки репитер запоминает идентификатор и соединение.
Когда на второй порт придет некое соединение, репитор дождется такой же строчки и идентификаторы совпадут, релей будет перенаправлять
сообщения с одного порта на другой. Перенаправление будет существовать до тех пор, пока одно из соединений не отвалится.
*/

// https://habr.com/ru/company/infopulse/blog/415259/

#include <iostream>
#include <exception>
#include <utility>
#include <vector>
#include <set>

#include <string.h>

#include "Socket.h"
#include "Listener.h"


int main(int argc, char** argv)
{
    int leftport = 54320;
    int rightport = 54321;

    for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-leftport") && i + 1 < argc)
			leftport = std::stoi(argv[i + 1]);
        else if (!strcmp(argv[i], "-rightport") && i + 1 < argc)
            rightport = std::stoi(argv[i + 1]);
	}

	try
	{
	    Listener listener(leftport, rightport);
	    listener.doWork();
	}
	catch(std::exception& _e)
	{
		std::cout << _e.what() << std::endl;
	}

    /*
    Socket sock1;
    Socket sock2(std::move(sock1));
    Socket sock2(sock1);
    Socket sock3;
    Socket sock4;

	std::set<Socket> sockList;
	sockList.insert(sock3);
    sockList.insert(sock4);
    sockList.erase(sock3);
    /**/

	//std::cin.get();
	return 0;
}