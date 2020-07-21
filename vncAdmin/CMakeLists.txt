cmake_minimum_required(VERSION 2.8)

project(vncAdmin)

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


add_executable(vncAdmin
				${CMAKE_CURRENT_SOURCE_DIR}/sdl_viewer.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/common_helpers.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/vnc_client.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/log_to_file.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
                )

if (MSVC)	
	target_link_libraries(vncAdmin
		SDL2-static
		#SDL2main
		vncserver
		vncclient
		ws2_32.lib
	)
else()
	target_link_libraries(vncAdmin
		SDL2-static
		#SDL2main
		vncserver
		vncclient
	)
endif()



#add_executable(vncView
#				${CMAKE_CURRENT_SOURCE_DIR}/../libvncserver/client_examples/SDLvncviewer.c
#                )
#if (MSVC)	
#	target_link_libraries(vncView
#		SDL2-static
#		#SDL2main
#		vncserver
#		vncclient
#		ws2_32.lib
#	)
#else()
#	target_link_libraries(vncView
#		SDL2-static
#		#SDL2main
#		vncserver
#		vncclient
#	)
#endif()