cmake_minimum_required(VERSION 2.8)

project(tcpRelay)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions( -std=c++14 )
elseif (MSVC)
    ## add appropriate keys
endif()

add_executable(tcpRelay
				src/main.cpp
				src/Socket.cpp
                src/socket_impl.cpp
				src/common.cpp
				src/Listener.cpp
				src/interface.cpp
				src/thread.cpp
				src/multiplexer.cpp
				src/ip_tools.cpp
                src/id_list.cpp
                src/msglist.cpp)

if (MSVC)
	target_link_libraries(tcpRelay
		ws2_32.lib
		)
else()
	target_link_libraries(tcpRelay
		pthread
		)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/ tests)