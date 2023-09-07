cmake_minimum_required(VERSION 3.16)

project(sh)

file(GLOB_RECURSE SOURCES "src/**.cpp" "src/**.hpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
target_link_libraries(${PROJECT_NAME} PRIVATE openal X11 GL pthread png)
