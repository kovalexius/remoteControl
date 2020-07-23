cmake_minimum_required(VERSION 2.8)

project(armAdmin)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.12/
						${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.12/cmake/
						${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.12/include/)

if(NOT TARGET vncserver AND NOT TARGET vncclient)
	add_subdirectory(
				${CMAKE_CURRENT_SOURCE_DIR}/../libvncserver/ libvncserver
				EXCLUDE_FROM_ALL )
endif()

if(NOT TARGET SDL2-static AND NOT TARGET SDL2main)
	set(SDL_STATIC_ENABLED_BY_DEFAULT ON)
	set(SDL_STATIC ON)
	add_definitions(-DSDL_MAIN_HANDLED)
	add_subdirectory(
				${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.12/ SDL2
				EXCLUDE_FROM_ALL)
	#remove_definitions(-DSDL_MAIN_HANDLED)
endif()

include_directories(
                ${CMAKE_CURRENT_SOURCE_DIR}/../libvncserver 
				${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.12/include/ 
                ${CMAKE_CURRENT_BINARY_DIR}/libvncserver/
				)

add_executable(armAdmin
				${CMAKE_CURRENT_SOURCE_DIR}/sdl_viewer.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/common_helpers.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/vnc_client.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/log_to_file.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
                )

if (MSVC)	
	target_link_libraries(armAdmin
		SDL2-static
		vncserver
		vncclient
		ws2_32.lib
	)
else()
	target_link_libraries(armAdmin
		SDL2-static
		vncserver
		vncclient
	)
endif()

add_subdirectory(SDLvncviewer vncViewer)