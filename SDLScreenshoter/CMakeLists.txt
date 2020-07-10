cmake_minimum_required(VERSION 2.8)

project(SDLCapturer)

include_directories(
				${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.12/include/)

# Перенести geometry_types.h в общую часть
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../JShot/jShot/src/)

add_library(SDLCapture STATIC 
                    ./sdlScrCapturer.cpp
                    ./sdlScrCapturer.h)

add_subdirectory(
				${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.12/ SDL2
				EXCLUDE_FROM_ALL)

target_link_libraries(SDLCapture
                            SDL2-static
	                        SDL2main)