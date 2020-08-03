#ifndef __WORKERS__H
#define __WORKERS__H

#include <string>
#include <future>

void writer(const std::string& _pattern, const std::string& _host, const int _port);
void reader(const std::string& _pattern, 
            const std::string& _host, 
            const int _port, 
            std::promise<bool> _testPassed);

#endif