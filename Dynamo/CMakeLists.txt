﻿# CMakeList.txt : CMake project for Dynamo, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
set(SRC
     "src/Core/Main.cpp"
     "src/Core/Window.cpp"
     "src/Core/App.cpp"
     "src/Graphics/Graphics.cpp"
     "src/Graphics/Model.cpp"
     "src/Debug/DynamoException.cpp"
     "src/Graphics/Gui.cpp" 
     "src/Graphics/Buffer.cpp"
     "src/Graphics/GPU.cpp"
    )

set(ASSIMP_DIR ../tools/assimp)
find_package(assimp)

set(IMGUI_DIR ../tools/imgui)

set(DXTEX_DIR ../tools/DirectXTex)
find_package(DirectXTex)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src 
                    ${IMGUI_DIR} 
                    ${IMGUI_DIR}/backends 
                    ${ASSIMP_DIR}/include
                    ${DXTEX_DIR}/DirectXTex)
set(IMGUI_SRC
      ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
      ${IMGUI_DIR}/backends/imgui_impl_dx11.cpp 
      ${IMGUI_DIR}/imgui.cpp 
      ${IMGUI_DIR}/imgui_draw.cpp 
      ${IMGUI_DIR}/imgui_demo.cpp 
      ${IMGUI_DIR}/imgui_tables.cpp 
      ${IMGUI_DIR}/imgui_widgets.cpp
    )

set(RESOURCE_SCRIPTS res/dynamo.rc)

add_executable(Dynamo WIN32 ${SRC} ${IMGUI_SRC} ${RESOURCE_SCRIPTS})
target_precompile_headers(Dynamo PRIVATE src/dynamopch.h)
target_link_libraries(Dynamo "d3d11.lib" "d3dcompiler.lib" "dxgi.lib" DirectXTex assimp)
add_custom_command(TARGET Dynamo POST_BUILD       
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        "${PROJECT_SOURCE_DIR}/tools/assimp/bin/Debug/assimp-vc143-mtd.dll"
        $<TARGET_FILE_DIR:Dynamo>)                