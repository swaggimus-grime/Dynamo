# CMAKE generated file: DO NOT EDIT!
# Generated by CMake Version 3.22
cmake_policy(SET CMP0009 NEW)

# PIXEL_SHADER_FILES at Dynamo/CMakeLists.txt:18 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/*ps.hlsl")
set(OLD_GLOB
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/AnimModelps.hlsl"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/Flatps.hlsl"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/Modelps.hlsl"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/Negativeps.hlsl"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/PointLightps.hlsl"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/Posps.hlsl"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/Skyboxps.hlsl"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/Solidps.hlsl"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "C:/Users/hunte/source/repos/Dynamo/CMakeFiles/cmake.verify_globs")
endif()

# VERTEX_SHADER_FILES at Dynamo/CMakeLists.txt:15 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/*vs.hlsl")
set(OLD_GLOB
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/AnimModelvs.hlsl"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/Flatvs.hlsl"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/Modelvs.hlsl"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/Negativevs.hlsl"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/PointLightvs.hlsl"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/Posvs.hlsl"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/ShadowMapvs.hlsl"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/Skyboxvs.hlsl"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/res/shaders/Solidvs.hlsl"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "C:/Users/hunte/source/repos/Dynamo/CMakeFiles/cmake.verify_globs")
endif()

# SRC at Dynamo/CMakeLists.txt:27 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/*.cpp")
set(OLD_GLOB
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Core/App.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Core/Input.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Core/Main.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Core/Window.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Debug/DynamoException.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/GUI/Gui.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/AnimModel.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Animation.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Animator.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/BlendState.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Bone.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Buffer.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Camera.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/DSState.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/DSView.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Graphics.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Light.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Model.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Negativepass.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Rasterizer.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/RenderTarget.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Sampler.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Scene.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Selector.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Shader.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Shadowpass.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Shapes.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Skybox.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Texture.cpp"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Transformable.cpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "C:/Users/hunte/source/repos/Dynamo/CMakeFiles/cmake.verify_globs")
endif()

# SRC at Dynamo/CMakeLists.txt:27 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/*.h")
set(OLD_GLOB
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Core/App.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Core/Input.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Core/Window.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Debug/DynamoException.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/GUI/GUIable.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/GUI/Gui.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/AnimModel.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Animation.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Animator.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/AssimpGLMHelpers.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Bindable.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/BlendState.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Bone.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Buffer.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Camera.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/DSState.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/DSView.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Graphics.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/GraphicsCore.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Light.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Mesh.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Model.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Negativepass.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Rasterizer.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/RenderTarget.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Renderable.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Renderpass.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Sampler.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Scene.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Selector.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Shader.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Shadowpass.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Shapes.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Skybox.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Texture.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/Transformable.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/Graphics/VectorOps.h"
  "C:/Users/hunte/source/repos/Dynamo/Dynamo/src/dynamopch.h"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "C:/Users/hunte/source/repos/Dynamo/CMakeFiles/cmake.verify_globs")
endif()
