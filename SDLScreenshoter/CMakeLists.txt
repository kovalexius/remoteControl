cmake_minimum_required(VERSION 2.8)

project(SDLCapturer)

include_directories(
				${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.10/include/)

add_library(SDLCapture STATIC 
                    ./sdlScrCapturer.cpp
                    ./sdlScrCapturer.h)

add_subdirectory(
				${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.12/ SDL2
				EXCLUDE_FROM_ALL)

target_link_libraries(SDLCapturer
                            SDL2-static
	                        SDL2main)