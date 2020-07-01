cmake_minimum_required(VERSION 2.8)

project(vncClient)

include_directories(
                ${CMAKE_CURRENT_SOURCE_DIR}/../libvncserver 
				${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.10/include/ 
                ${CMAKE_CURRENT_BINARY_DIR}/libvncserver/
				)
                
add_subdirectory(${CMAKE_SOURCE_DIR}/../libvncserver/ libvncserver
				EXCLUDE_FROM_ALL
				)

link_directories(
                ${CMAKE_CURRENT_BINARY_DIR}/libvncserver/
				${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.10/lib/x64/
                )


add_executable(client
				${CMAKE_CURRENT_SOURCE_DIR}/SDLvncviewer.cpp
                )
				
target_link_libraries(client
	SDL2.lib
	SDL2main.lib
	vncserver.lib
	vncclient.lib
	ws2_32.lib
)