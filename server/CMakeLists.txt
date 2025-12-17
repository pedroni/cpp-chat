file(GLOB SOURCES "*.cpp")

add_executable(ChatServer ${SOURCES})

# fmt
find_package(fmt REQUIRED)

target_link_libraries(ChatServer PRIVATE fmt::fmt)
