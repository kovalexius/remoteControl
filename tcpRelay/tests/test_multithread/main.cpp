// http://rus-linux.net/MyLDP/consol/tcp-udp-socket-bash-shell.html
// Написать сокеты на bash'е

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <future>
#include <memory>
#include <thread>
#include <mutex>

#include <string.h>

#include "workers.h"

using Result = std::tuple<std::thread,      /* read thread */
                            std::thread,        /* write thread */
                            std::future<bool>   /* result of testing */ >;

void RunTestTask(const std::string& _pattern, 
                const std::string& _host, 
                const int _port1,
                const int _port2,
                std::vector<Result>& _resultHolder)
{
    std::promise<bool> promise;
    std::future<bool> result_future = promise.get_future();

    std::thread wrThr(&writer, _pattern, _host, _port1);
    std::thread readThr(&reader, _pattern, _host, _port2, std::move(promise));
    Result resTuple(std::move(wrThr), std::move(readThr), std::move(result_future));
    _resultHolder.emplace_back(std::move(resTuple));
}

int main(int argc, char** argv)
{
    int leftport = 54320;
    int rightport = 54321;
    std::string host("localhost");

    for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-leftport") && i + 1 < argc)
			leftport = std::stoi(argv[i + 1]);
        else if (!strcmp(argv[i], "-rightport") && i + 1 < argc)
            rightport = std::stoi(argv[i + 1]);
        else if (!strcmp(argv[i], "-host") && i + 1 < argc)
            host = argv[i+1];
	}

    std::vector<std::string> patterns = 
    {
        "abcd",
        "1234",
        "qwer",
        "11qq",
        "5r4e",
        "jjhh",
        "priv",
        "halo",
        "FuCK",
        "5500",
        "4321", 
        "p@$$word",
        "root",
        "admin",
        "Admin",
        "Test",
        "test",
    };

    

    std::vector<Result> resultHolder;

    RunTestTask("arm1", host, leftport, rightport, resultHolder);

    /*
    for(auto i = 0; i < patterns.size(); i++)
    {
        if(i%2)
            RunTestTask(patterns[i], host, rightport, leftport, resultHolder);
        else
            RunTestTask(patterns[i], host, leftport, rightport, resultHolder);
    }

    for(auto& item : resultHolder)
    {
        if(!std::get<2>(item).get())
            std::cerr << "Test failed" << std::endl;
        else
            std::cout << "Test passed" << std::endl;
    }

    for(auto& item : resultHolder)
    {
        std::get<0>(item).join();
        std::get<1>(item).join();
    }
    */

    return 0;
}