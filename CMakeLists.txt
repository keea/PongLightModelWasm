# emcc 3.1.43
cmake_minimum_required(VERSION 3.0.0)
project(PongLightModelWasm VERSION 0.1.0)

# "Pure" C++17 and no extensions.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE CORE_HDR ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE CORE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

include(FetchContent)

if(NOT EMSCRIPTEN)
  FetchContent_Declare(
    glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw"
    GIT_TAG 3.3.8
  )
  add_subdirectory(3rdparty/glfw)
endif()

FetchContent_Declare(
  glm
  GIT_REPOSITORY "https://github.com/g-truc/glm.git"
  GIT_TAG 0.9.9.8
)
add_subdirectory(3rdparty/glm)

FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v0.1.36
)
add_subdirectory(3rdparty/glad)

include(FetchContent)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
  GIT_TAG v1.89.7
)
add_subdirectory(3rdparty/imgui)

if(NOT EMSCRIPTEN)
  find_package(OpenGL REQUIRED)
  include_directories(${OPENGL_INCLUDE_DIRS})
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(${PROJECT_NAME} ${CORE_HDR} ${CORE_SRC})

if(EMSCRIPTEN)
  set(OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/output/web)
  target_link_libraries(${PROJECT_NAME} glm glad imgui)
  set_target_properties(${PROJECT_NAME}
    PROPERTIES 
    SUFFIX ".html"
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
    LINK_FLAGS "--shell-file ${CMAKE_CURRENT_SOURCE_DIR}/res/template/template.html -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -O3 -std=c++14 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/res/shaderFiles/esShader@Shader")
    # 빌드 후에 HTML 파일 이름 변경
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rename ${OUTPUT_DIR}/${PROJECT_NAME}.html ${OUTPUT_DIR}/index.html)
else()
  set(OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/output/app)
  target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw glm glad imgui)
  set_target_properties(${PROJECT_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res/shaderFiles/shader DESTINATION ${OUTPUT_DIR})
endif()
