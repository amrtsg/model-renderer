# 3D Graphics Engine C++/OpenGL

### Build

Create a folder called "build" in the project file, then create a VS solution using CMake

![alt text](https://github.com/amrtsg/model-renderer/tree/master/screenshots/githubpics/cmake.png)

After opening the solution, be sure to set "src" as the startup project, and then do the following

 - ADD "assimp-vc140-mt.lib" to Linker -> Input -> Additional Dependencies
 - Copy the files in external/assimp/lib  - TO -> build/src

NOTE: WILL CHANGE THE LINKS FROM ABSOLUTE TO RELATIVE NEXT PUSH, FOR NOW JUST CHANGE THEM YOURSELF

# Functionality

### Terrain

![alt text](https://github.com/amrtsg/GameEngineCPP/blob/master/terrain.png?raw=true)

### Skybox

![alt text](https://github.com/amrtsg/GameEngineCPP/blob/master/skybox.png?raw=true)

### .OBJ File rendering using ASSIMP

![alt text](https://github.com/amrtsg/GameEngineCPP/blob/master/3dobj.png?raw=true)

# Dependencies (All Included)

 - Assimp
 - OpenGL
 - GLFW
 - GLEW
 - GLM
 - STB_IMAGE

# Next Update

 - Currently working on water tiles, right now they are just a blue plane.
 - Fixing bug where terrain wont generate with other objects in the scene.
 - Cleaning up code (I know it is messy as of now, bare with me)
 - Changing absolute links to relative.
