cmake_minimum_required(VERSION 2.8)

project(vncViewer)

if(NOT TARGET vncserver AND NOT TARGET vncclient)
	add_subdirectory(
				${CMAKE_CURRENT_SOURCE_DIR}/../../libvncserver/ libvncserver
				EXCLUDE_FROM_ALL )
endif()

if(NOT TARGET SDL2-static AND NOT TARGET SDL2main)
	set(SDL_STATIC_ENABLED_BY_DEFAULT ON)
	set(SDL_STATIC ON)
	add_definitions(-DSDL_MAIN_HANDLED)
	add_subdirectory(
				${CMAKE_CURRENT_SOURCE_DIR}/../../SDL2-2.0.12/ SDL2
				EXCLUDE_FROM_ALL)
endif()

include_directories(
                ${CMAKE_CURRENT_SOURCE_DIR}/../../libvncserver 
				${CMAKE_CURRENT_SOURCE_DIR}/../../SDL2-2.0.12/include/ 
                ${CMAKE_CURRENT_BINARY_DIR}/libvncserver/
				)
				
add_executable(SDLvncviewer
				${CMAKE_CURRENT_SOURCE_DIR}/SDLvncviewer.c
                )
				
if (MSVC)	
	target_link_libraries(SDLvncviewer
		SDL2-static
		vncserver
		vncclient
		ws2_32.lib
	)
else()
	target_link_libraries(SDLvncviewer
		SDL2-static
		vncserver
		vncclient
	)
endif()