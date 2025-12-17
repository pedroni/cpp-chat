file(GLOB SOURCES "*.cpp" "utils/*.cpp")

add_executable(ChatClient ${SOURCES})

# CURSES for terminal management
# https://cmake.org/cmake/help/latest/module/FindCurses.html
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

# https://cmake.org/cmake/help/latest/module/FindCurses.html
# https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/intro.html#WHATIS
add_library(Curses::Curses INTERFACE IMPORTED)
set_target_properties(Curses::Curses
        PROPERTIES
            INTERFACE_LINK_LIBRARIES "${CURSES_LIBRARIES}"
            INTERFACE_INCLUDE_DIRECTORIES "${CURSES_INCLUDE_DIRS}"
    )


# fmt
find_package(fmt REQUIRED)

target_link_libraries(ChatClient PRIVATE Curses::Curses fmt::fmt)
