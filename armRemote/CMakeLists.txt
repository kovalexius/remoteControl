cmake_minimum_required(VERSION 2.8)

project(armRemote)


if (_MSC_VER LESS 1900)
    MESSAGE(STATUS "_MSC_VER < 1900")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

	add_definitions(
		#-D_NO_CRT_STDIO_INLINE
		#-D__STDC_WANT_LIB_EXT1__=1
		-Dsnprintf=_snprintf
		#/NODEFAULTLIB:library
		)

	#set(CMAKE_CXX_FLAGS_RELEASE "/MT")
	#set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

	#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libvncserver/
				${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.10/include/ 
				${CMAKE_CURRENT_BINARY_DIR}/libvncserver/
				)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../libvncserver/ libvncserver
				EXCLUDE_FROM_ALL
				)

link_directories(
				${CMAKE_CURRENT_BINARY_DIR}/libvncserver/
				)
				


add_executable(armRemote
				${CMAKE_CURRENT_SOURCE_DIR}/vncServer.cpp
        
        )

if (_MSC_VER)			
	target_compile_options(armRemote
						PRIVATE /W3
					)
endif()		

if (_MSC_VER)			
	target_link_libraries(armRemote
		vncserver
		vncclient
		ws2_32.lib
		)
else()
	target_link_libraries(armRemote
		vncserver
		vncclient
		pthread
		)
endif()