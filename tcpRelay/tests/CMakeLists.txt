cmake_minimum_required(VERSION 2.8)

project(tests)

add_executable(test_sockets
                ${CMAKE_CURRENT_SOURCE_DIR}/test_sockets.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/../src/Socket.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/../src/socket_impl.cpp)

target_include_directories(test_sockets SYSTEM
                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/)

if (MSVC)
    target_link_libraries(test_sockets
                    ws2_32.lib)
else()
endif()


if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Werror)
endif()

add_executable(testTcpRelay
                ${CMAKE_CURRENT_SOURCE_DIR}/test_multithread/main.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/test_multithread/workers.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/../src/Socket.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/../src/socket_impl.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/../src/common.cpp)

target_include_directories(testTcpRelay SYSTEM
                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/)

if(UNIX)
    target_link_libraries(testTcpRelay
            pthread)
endif()