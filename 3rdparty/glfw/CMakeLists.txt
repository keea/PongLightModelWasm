message(STATUS "Fetching glfw ...")

set(CMAKE_CXX_STANDARD 17)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)

    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif() 
