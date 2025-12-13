cmake_minimum_required(VERSION 3.23)

project(Chat)

file(GLOB SOURCES "src/*.cpp")

add_executable(ChatExecutable ${SOURCES})
