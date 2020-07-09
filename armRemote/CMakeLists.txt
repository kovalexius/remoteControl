cmake_minimum_required(VERSION 2.8)

project(armRemote)

if (DEFINED ${MSVC_VERSION})
	if (${MSVC_VERSION} LESS 1900)
		add_definitions(
			-Dsnprintf=_snprintf
			)
	endif()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libvncserver/
				${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.10/include/ 
				
				${CMAKE_CURRENT_BINARY_DIR}/libvncserver/
				)

if(WIN32)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../JShot/jShot/src/)
elseif(LINUX)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../JShot/jShot/src/)

if(NOT TARGET vncserver AND NOT TARGET vncclient)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../libvncserver/ libvncserver
				EXCLUDE_FROM_ALL)
endif()

if(WIN32)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../JShot/jShot/ jShot
					EXCLUDE_FROM_ALL)
elseif(LINUX)
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

#if (MSVC)
#	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
#	target_compile_options(armRemote
#						PRIVATE /W3
#					)
#	set(CMAKE_CXX_FLAGS_RELEASE "/MT")
#	set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
#	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
#	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
#endif()		

if (MSVC)			
	target_link_libraries(armRemote
		vncserver
		vncclient
		jShot
		ws2_32.lib
		)
else()
	target_link_libraries(armRemote
		vncserver
		vncclient
		pthread
		)
endif()