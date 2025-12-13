cmake_minimum_required(VERSION 3.23)

project(Chat)

# CURSES for terminal management
# https://cmake.org/cmake/help/latest/module/FindCurses.html
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

# https://cmake.org/cmake/help/latest/module/FindCurses.html
# https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/intro.html#WHATIS
if (Curses_FOUND)
    add_library(Curses::Curses INTERFACE IMPORTED)
    set_target_properties(Curses::Curses
        PROPERTIES
            INTERFACE_LINK_LIBRARIES "${CURSES_LIBRARIES}"
            INTERFACE_INCLUDE_DIRECTORIES "${CURSES_INCLUDE_DIRS}"
    )
else()
    message("curses not found: https://cmake.org/cmake/help/latest/module/FindCurses.html")
endif()

file(GLOB SOURCES "src/*.cpp")

add_executable(ChatExecutable ${SOURCES})
target_link_libraries(ChatExecutable PRIVATE Curses::Curses)
