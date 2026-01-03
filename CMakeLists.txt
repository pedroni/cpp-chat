cmake_minimum_required(VERSION 3.23)

project(Chat)

add_compile_options(-Wshadow)

# these is like adding a new project and having a "monorepo" maybe?
add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)
