cmake_minimum_required(VERSION 2.8)

project(remoteControl)

add_subdirectory(vncAdmin/ vncAdmin)

add_subdirectory(armRemote/ armRemote)

#ExternalProject_Add  Команда, скачивающая репозитории