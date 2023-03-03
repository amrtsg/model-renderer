# 3D Model Renderer C++/OpenGL

### Build

Create a folder called "build" in the project folder, then create a VS solution using CMake

![alt text](https://github.com/amrtsg/model-renderer/blob/master/screenshots/githubpics/cmake.png?raw=true)

After opening the solution, be sure to set "src" as the startup project, and then do the following

 - ADD "assimp-vc140-mt.lib" to Linker -> Input -> Additional Dependencies
 - Copy the files in external/assimp/lib  - TO -> build/src

# Functionality

![alt text](https://github.com/amrtsg/model-renderer/blob/master/screenshots/githubpics/showcase.gif?raw=true)

# Dependencies (All Included)

 - Assimp
 - OpenGL
 - GLFW
 - GLEW
 - GLM
 - STB_IMAGE
 - ImGui
