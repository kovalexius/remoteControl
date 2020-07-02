cmake_minimum_required(VERSION 2.8)

#project(remoteControl)

add_subdirectory(vncAdmin/ vncAdmin
    EXCLUDE_FROM_ALL)

add_subdirectory(armRemote/ armRemote
    EXCLUDE_FROM_ALL)