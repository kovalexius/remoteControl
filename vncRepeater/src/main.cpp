/*
relay(репитер) слушает два разных порта.
Когда на какой-либо порт приходит новое соединение, то репитер ожидает от него строчки вида ID:<какой-то идентификатор>
Остальные сообщения игнорируются (куда нибудь логируется как ошибка)
По приходу ожидаемой строки репитер запоминает идентификатор и соединение.
Когда на второй порт придет некое соединение, репитор дождется такой же строчки и идентификаторы совпадут, релей будет перенаправлять
сообщения с одного порта на другой. Перенаправление будет существовать до тех пор, пока одно из соединений не отвалится.
*/

#include <iostream>
#include <exception>
#include <utility>
#include <vector>

#include "Socket.h"
#include "Listener.h"


int main()
{
	/*
	try
	{
	Listener listener(54320, 54321);
	listener.doWork();

	std::cin.get();
	}
	catch(std::exception& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	*/

	Socket sock1;
	Socket sock2(std::move(sock1));
	Socket sock3;
	Socket sock4;

	std::vector<Socket> sockList;
	sockList.push_back(std::move(sock4));


	std::cin.get();
	return 0;
}