# Dynamo 
### <sub> _Dynamic Modeler_ </sub>
3D game engine using Direct3D.

## Features
- Scene support 
- Point Lights
- Model loading(.dae, .gltf, and .fbx have been tested)
- Skybox
- Render Dependency Graph
- Dockable windows

## Build
This project can be built using CMake(min. version 3.8). 
```
cmake -B build
```

## Screenshots
![scene](/screenshots/scene.png)
![Skybox](/screenshots/skybox.png)
![RDG](/screenshots/rdg.png)

## Dependencies
- Direct3D(for backend rendering)</br>
- [Assimp](https://github.com/assimp/assimp)(for model loading)</br>
- [DirectXTex](https://github.com/microsoft/DirectXTex)(for image processing)</br>
- [Dear ImGUI](https://github.com/ocornut/imgui)(for GUI)</br>
- [imgui-node-editor](https://github.com/thedmd/imgui-node-editor)
