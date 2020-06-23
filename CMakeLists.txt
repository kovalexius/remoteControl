cmake_minimum_required(VERSION 2.8)

project(VNC)

include_directories(../libvncserver 
				../SDL2-2.0.10/include/ 
				../libvncserver-build)

link_directories(../libvncserver-build/Release/
				../SDL2-2.0.10/lib/x64/)


add_executable(client
				vncClient/SDLvncviewer.c)
				
target_link_libraries(client
	SDL2.lib
	SDL2main.lib
	vncserver.lib
	vncclient.lib
	ws2_32.lib
)
				
add_executable(server
				vncServer/vncServer.c)
				
target_link_libraries(server
	vncserver.lib
	ws2_32.lib)
	
	
#add_subdirectory(vncClient)
#add_subdirectory(vncServer)