cmake_minimum_required(VERSION 2.8)

project(X11Screenshoter)

add_library(X11Capture STATIC
				${CMAKE_CURRENT_SOURCE_DIR}/x11_scr_capture_impl.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/x11_scr_capture_impl.h
				)

target_link_libraries(X11Capture
							-lX11
							)