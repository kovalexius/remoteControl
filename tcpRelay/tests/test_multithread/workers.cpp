#include "workers.h"

#include <exception>

#include <iostream>

#include <sys/socket.h>
#include <poll.h>
#include <unistd.h>
#include <string.h>

#include "../../src/Socket.h"
#include "../../src/common.h"



constexpr int REPTIMES = 10000000;
constexpr int PIECESIZE = 8192;

std::string generateMsg(const std::string& _pattern, const int _repTimes)
{
    std::string result;
    for(auto i = 0; i < _repTimes; i++)
        result += _pattern;
    return result;
}

void writer(const std::string& _pattern, const std::string& _host, const int _port)
{
    std::cout << __FUNCTION__ << " tid: " << std::this_thread::get_id() << 
                " host: " << _host << " _port: " << _port << std::endl;

    try
    {
        std::string outBuffer = generateMsg(_pattern, REPTIMES);
        auto numpieces = outBuffer.size() / PIECESIZE;

        Socket socket;
        connect_host(socket, _host, _port);
        //bind_socket(socket, std::string(), _port);

        std::string id_string = std::string("ID:") + _pattern;
        write(socket.Get(), id_string.data(), id_string.size());

        int i;
        for(i = 0; i < numpieces; i++)
            write(socket.Get(), outBuffer.data() + i * PIECESIZE, PIECESIZE);
        if(outBuffer.size() % PIECESIZE > 0)
            write(socket.Get(), outBuffer.data() + i * PIECESIZE, outBuffer.size() % PIECESIZE);

        shutdown(socket.Get(), SHUT_RDWR);
    }
    catch(std::exception& _e)
    {
        std::cerr << "Caught exception: " << _e.what() << std::endl;
    }

    std::cout << "End of " << __FUNCTION__ << std::endl;
}

void reader(const std::string& _pattern, 
            const std::string& _host, 
            const int _port, 
            std::promise<bool> _testPassed)
{
    std::cout << __FUNCTION__ << " tid: " << std::this_thread::get_id() << 
                " host: " << _host << " _port: " << _port << std::endl;

    try
    {
        std::string fullBuffer;

        Socket socket;
        connect_host(socket, _host, _port);
        //bind_socket(socket, std::string(), _port);

        std::string id_string = std::string("ID:") + _pattern;
        write(socket.Get(), id_string.data(), id_string.size());

        struct pollfd fds[1];
	    fds[0].fd = socket.Get();
	    fds[0].events = POLLIN;
        while(true)
        {
            auto res = poll(fds, 1, TIMEOUT);
            if(res < 0)
            {
                std::cout << "poll failed: \'" << strerror(errno) << "\'" << std::endl;
                break;
            }
            else if(res == 0)
            {}
            else
            {
                if(fds[0].revents & POLLIN)
			    {
				    fds[0].revents = 0;
				    std::string msg;
                    read_socket(socket, msg);
                    if(msg.size() == 0)
                        break;
                    fullBuffer += msg;
			    }

                if(fds[0].revents & POLLHUP || fds[0].revents & POLLRDHUP || fds[0].revents & POLLERR)
                {
                    std::cout << "poll hup: " << strerror(errno) << std::endl;
                    break;
                }
            }
        }

        std::string requiredBuffer = generateMsg(_pattern, REPTIMES);
        if(fullBuffer == requiredBuffer)
            _testPassed.set_value(true);
        else
            _testPassed.set_value(false);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Caught exception: " << e.what() << std::endl;
        _testPassed.set_value(false);
    }
    _testPassed.set_value(false);
}