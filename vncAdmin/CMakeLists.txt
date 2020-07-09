cmake_minimum_required(VERSION 2.8)

project(vncAdmin)

if(NOT TARGET vncserver AND NOT TARGET vncclient)
	add_subdirectory(
				${CMAKE_CURRENT_SOURCE_DIR}/../libvncserver/ libvncserver
				EXCLUDE_FROM_ALL )
endif()
				
add_subdirectory(
				${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.12/ SDL2
				EXCLUDE_FROM_ALL
				)

include_directories(
                ${CMAKE_CURRENT_SOURCE_DIR}/../libvncserver 
				${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.12/include/ 
                ${CMAKE_CURRENT_BINARY_DIR}/libvncserver/
				)
                


link_directories(
                ${CMAKE_CURRENT_BINARY_DIR}/libvncserver/
				${CMAKE_CURRENT_BINARY_DIR}/SDL2/
                )


add_executable(vncAdmin
				${CMAKE_CURRENT_SOURCE_DIR}/SDLvncviewer.cpp
                )
				
target_link_libraries(vncAdmin
	SDL2-static
	SDL2main
	vncserver
	vncclient
	ws2_32.lib
)