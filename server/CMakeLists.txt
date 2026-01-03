file(GLOB SOURCES "*.cpp")

add_executable(ChatServer ${SOURCES})

target_link_libraries(ChatServer PRIVATE common)

# fmt
find_package(fmt REQUIRED)

target_link_libraries(ChatServer PRIVATE fmt::fmt)
