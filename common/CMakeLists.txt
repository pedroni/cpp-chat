file(GLOB COMMON_SOURCES "src/*.cpp")
# list out files that need to be compiled
add_library(common STATIC
    ${COMMON_SOURCES}
)

# this will make available all the .h files that are present in the common project/library, PUBLIC means that .h will be available for consumers and for the the common itself
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
