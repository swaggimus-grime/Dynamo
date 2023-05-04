# Dynamo 
### <sub> _Dynamic Modeler_ </sub>
3D modeling engine using Direct3D. 

## Controls
- WASD: Move camera in cardinal directions
- Q/E: Move camera up/down
- Tab: Toggle mouse movement

## Features
- Scene support 
- Point Lights
- Model loading(.dae, .gltf, and .fbx have been tested)
- Skybox
- Render Dependency Graph
- Dockable windows

## Compatability
The project has only been tested on Windows 10. Uses x64 architecture.

## Build
This project can be built using CMake(min. version 3.8). 
```
cmake -B build
```
Note for using CMake for Visual Studio: please set "currentDir" to ${workspaceRoot}\\Dynamo in launch.vs.json

## Screenshots
![scene](/screenshots/scene.png)
![Skybox](/screenshots/skybox.png)
![RDG](/screenshots/rdg.PNG)

## Dependencies
- Direct3D(for backend rendering)</br>
- [Assimp](https://github.com/assimp/assimp)(for model loading)</br>
- [DirectXTex](https://github.com/microsoft/DirectXTex)(for image processing)</br>
- [Dear ImGUI](https://github.com/ocornut/imgui)(for GUI)</br>
- [imgui-node-editor](https://github.com/thedmd/imgui-node-editor)
