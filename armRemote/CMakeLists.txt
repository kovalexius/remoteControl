cmake_minimum_required(VERSION 2.8)

project(armRemote)

#set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.12/
#						${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.12/cmake/
#						${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.12/include/)

if (DEFINED ${MSVC_VERSION})
	if (${MSVC_VERSION} LESS 1900)
		add_definitions(
			-Dsnprintf=_snprintf
			)
	endif()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libvncserver/
				${CMAKE_CURRENT_SOURCE_DIR}/../SDLScreenshoter/
				${CMAKE_CURRENT_BINARY_DIR}/libvncserver/
				)

# Перенести geometry_types.h в общую часть
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../JShot/jShot/src/)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../SDLScreenshoter/ SDLScreenshoter
					)

if(WIN32)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../JShot/jShot/src/)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../JShot/jShot/ jShot
					EXCLUDE_FROM_ALL)
endif()

if(NOT TARGET vncserver AND NOT TARGET vncclient)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../libvncserver/ libvncserver
				EXCLUDE_FROM_ALL)
endif()



add_executable(armRemote
				${CMAKE_CURRENT_SOURCE_DIR}/server.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/server.h
				${CMAKE_CURRENT_SOURCE_DIR}/server_holder.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/server_holder.h
				${CMAKE_CURRENT_SOURCE_DIR}/screenshot_poly.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/screenshot_poly.h
				${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
				)		

if (MSVC)
	target_link_libraries(armRemote
		vncserver
		vncclient
		SDLCapture
		jShot
		ws2_32.lib
		)
else()
	target_link_libraries(armRemote
		vncserver
		vncclient
		SDLCapture
		pthread
		)
endif()