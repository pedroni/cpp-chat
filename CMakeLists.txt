cmake_minimum_required(VERSION 3.23)

project(Chat)

# these is like adding a new project and having a "monorepo" maybe?
add_subdirectory(client)
add_subdirectory(server)
