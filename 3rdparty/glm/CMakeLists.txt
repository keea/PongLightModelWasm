message(STATUS "Fetching glm ...")

set(CMAKE_CXX_STANDARD 17)

FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
    set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
    add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
endif()
