cmake_minimum_required(VERSION 3.10)

project ("OBJRenderer")

#--- Load the common configuration
include(common/config.cmake)

#--- OpenGL configuration
include(common/OpenGL.cmake)
include(common/GLEW.cmake)
include(common/GLFW.cmake)

#---include directories-----
include_directories(external/glm)
include_directories(src/headers)
include_directories(external/stb_image)
include_directories(external/imgui)
include_directories(external/assimp/include)

add_subdirectory(src)