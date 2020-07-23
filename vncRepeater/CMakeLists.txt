cmake_minimum_required(VERSION 2.8)

project(remoteControlRelay)

add_executable(remoteControlRelay
				src/main.cpp
				src/Socket.cpp
				src/Socket.h)

if (MSVC)
	target_link_libraries(remoteControlRelay
		ws2_32.lib
		)
else()
	target_link_libraries(remoteControlRelay
		pthread
		)
endif()