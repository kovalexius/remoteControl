cmake_minimum_required(VERSION 2.8)

project(armRemote)

#set(CMAKE_CXX_FLAGS_RELEASE "/MT")
#set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libvncserver/
				${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.10/include/ 
				#${CMAKE_CURRENT_SOURCE_DIR}/../../libvncserver-build
				${CMAKE_CURRENT_BINARY_DIR}/libvncserver/
				)

add_subdirectory(${CMAKE_SOURCE_DIR}/../libvncserver/ libvncserver
				EXCLUDE_FROM_ALL
				)

link_directories(
				#${CMAKE_CURRENT_SOURCE_DIR}/../../libvncserver-build/Release/
				${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.0.10/lib/x64/
				${CMAKE_CURRENT_BINARY_DIR}/libvncserver/
				)
				
#add_definitions(
#-D_NO_CRT_STDIO_INLINE
#-D__STDC_WANT_LIB_EXT1__=1)

add_executable(armRemote
				${CMAKE_CURRENT_SOURCE_DIR}/vncServer.c)
				
target_compile_options(armRemote
	PRIVATE /W3
)			
				
target_link_libraries(
	armRemote
	vncserver
	vncclient
	ws2_32.lib
	MSVCRT.lib
	)