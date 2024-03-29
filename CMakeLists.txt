﻿# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE "Debug")
set(WIN32 true)

project ("Dynamo")

# Include sub-projects.
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(NOT GIT_SUBMODULE)
        message(FATAL_ERROR "Failed to turn on GIT_SUBMODULE")
    endif()
endif()

message(STATUS "Working directory: ${PROJECT_SOURCE_DIR}")

function(add_git_submodule dir)
    message(STATUS "Submodule update ${dir}")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive ${dir}
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init --recursive ${dir} failed with ${GIT_SUBMOD_RESULT}")
    endif()
endfunction(add_git_submodule)

add_git_submodule("tools/assimp")
add_git_submodule("tools/DirectXTex")
add_git_submodule("tools/imgui")
add_git_submodule("tools/node-editor")

add_subdirectory("tools/assimp")
add_subdirectory ("Dynamo")
add_subdirectory("tools/DirectXTex")

set_target_properties(assimp
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:Dynamo>
)

install(TARGETS Dynamo assimp DirectXTex DESTINATION bin)