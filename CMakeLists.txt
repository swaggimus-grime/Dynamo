# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

project ("Dynamo")

# Include sub-projects.

add_subdirectory ("Dynamo")
add_subdirectory("tools/assimp")
add_subdirectory("tools/DirectXTex")

install(TARGETS Dynamo assimp DirectXTex DESTINATION bin)