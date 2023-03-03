#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <gtc/type_ptr.hpp>

using namespace std;

class Shader
{
public:
    GLuint Program;
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    void use();
    void loadClipPlane(glm::vec4 clipPlane);
    void setUniforms(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
    void setUniforms(glm::mat4 view, glm::mat4 projection);
    void loadWaterTextures();
    void loadFogVariables(glm::vec3 skyColour, float density, float gradient);
    void loadSkyboxVariables(glm::vec3 skyColour, float upperLimit, float lowerLimit);
    void loadLightVariables(glm::vec3 position, glm::vec3 colour);
    void loadTerrainTextures();
    void useBlendMap(bool useBlendMap);
};