cmake_minimum_required(VERSION 2.8)

project(SDLCapturer)

include_directories(
				#${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.12/include/
                ${CMAKE_CURRENT_SOURCE_DIR}/../libvncserver 
				${CMAKE_CURRENT_BINARY_DIR}/SDL2/include 
                ${CMAKE_CURRENT_BINARY_DIR}/libvncserver/
				)


# Перенести geometry_types.h в общую часть
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../JShot/jShot/src/)

add_library(SDLCapture STATIC 
                    ./sdlScrCapturer.cpp
                    ./sdlScrCapturer.h)


if(NOT TARGET SDL2-static AND NOT TARGET SDL2main)
	set(SDL_STATIC_ENABLED_BY_DEFAULT ON)
	set(SDL_STATIC ON)
	add_subdirectory(
				${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.12/ SDL2
				#EXCLUDE_FROM_ALL
				)
endif()


target_link_libraries(SDLCapture
                            SDL2-static
	                        SDL2main
							)