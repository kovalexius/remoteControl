cmake_minimum_required(VERSION 2.8)

project(vncServer)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../libvncserver 
				${CMAKE_CURRENT_SOURCE_DIR}/../../SDL2-2.0.10/include/ 
				${CMAKE_CURRENT_SOURCE_DIR}/../../libvncserver-build)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../libvncserver-build/Release/
				${CMAKE_CURRENT_SOURCE_DIR}/../../SDL2-2.0.10/lib/x64/)
				
add_definitions(
-D_NO_CRT_STDIO_INLINE
-D__STDC_WANT_LIB_EXT1__=1)

add_executable(server
				${CMAKE_CURRENT_SOURCE_DIR}/vncServer.c)
				
target_link_libraries(server
	vncserver.lib
	ws2_32.lib)