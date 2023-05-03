# CMakeList.txt : CMake project for Dynamo, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

set(ASSIMP_DIR ../tools/assimp)
set(IMGUI_DIR ../tools/imgui)
set(NODE_EDITOR_DIR ../tools/node-editor)
set(GLM_DIR ../tools/glm)
set(DXTEX_DIR ../tools/DirectXTex)

file(GLOB_RECURSE VERTEX_SHADER_FILES CONFIGURE_DEPENDS
  "res/shaders/*vs.hlsl")

file(GLOB_RECURSE PIXEL_SHADER_FILES CONFIGURE_DEPENDS
  "res/shaders/*ps.hlsl")

set(SHADER_FILES 
    ${VERTEX_SHADER_FILES}
    ${PIXEL_SHADER_FILES}
    )

# Add source to this project's executable.
file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src 
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics
                    ${IMGUI_DIR} 
                    ${IMGUI_DIR}/backends 
                    ${NODE_EDITOR_DIR}
                    ${GLM_DIR}
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

set(NODE_EDITOR_SRC
      ${NODE_EDITOR_DIR}/crude_json.cpp
      ${NODE_EDITOR_DIR}/imgui_canvas.cpp
      ${NODE_EDITOR_DIR}/imgui_node_editor.cpp
      ${NODE_EDITOR_DIR}/imgui_node_editor_api.cpp
    )

set(RESOURCE_SCRIPTS res/dynamo.rc)

add_executable(Dynamo WIN32 ${SRC} ${IMGUI_SRC} ${NODE_EDITOR_SRC} ${RESOURCE_SCRIPTS})
target_precompile_headers(Dynamo PRIVATE src/dynamopch.h)
target_link_libraries(Dynamo "d3d11.lib" "d3dcompiler.lib" "dxgi.lib" DirectXTex assimp::assimp)  
set_target_properties(
    Dynamo PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "Working dir: ${VS_DEBUGGER_WORKING_DIRECTORY}")
# set_source_files_properties(${VERTEX_SHADER_FILES} PROPERTIES VS_SHADER_TYPE "vs")
# set_source_files_properties(${PIXEL_SHADER_FILES} PROPERTIES VS_SHADER_TYPE "ps")
# set_source_files_properties(${SHADER_FILES} PROPERTIES VS_SHADER_MODEL "5_0")
# 
# foreach(FILE ${SHADER_FILES})
# get_filename_component(FILE_WE ${FILE} NAME_WE)
# get_source_file_property(shadertype ${FILE} VS_SHADER_TYPE)
# get_source_file_property(shadermodel ${FILE} VS_SHADER_MODEL)
# add_custom_command(TARGET Dynamo POST_BUILD
#                      COMMAND fxc.exe /nologo /Emain /T${shadertype}_${shadermodel} $<$<CONFIG:DEBUG>:/Od> /Zi /Fo res/shaders/${FILE_WE}.cso ${FILE}
#                      MAIN_DEPENDENCY ${FILE}
#                      DEPENDS ${FILE}
#                      COMMENT "HLSL ${FILE}"
#                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#                      VERBATIM)
# endforeach(FILE)