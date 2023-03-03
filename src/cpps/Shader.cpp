#include <Shader.h>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
    // 1. Retrieve the vertex/fragment source code from filePath
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;
    // ensures ifstream objects can throw exceptions:
    vShaderFile.exceptions(ifstream::badbit);
    fShaderFile.exceptions(ifstream::badbit);
    try
    {
        // Open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (ifstream::failure e) {
        cout << "SHADER PROGRAM ERROR " << endl;
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();
    // 2. Compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // Print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        cout << "VERTEX ERROR " << infoLog << endl;
    }


    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // Print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        cout << "FRAGMENT ERROR " << infoLog << endl;
    }
    // Shader Program
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);
    // Print linking errors if any
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
        cout << "SHADER LINKING ERROR " << infoLog << endl;
    }
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

// Uses the current shader
void Shader::use() {
    glUseProgram(this->Program);
}

//pass uniform values to variables in shader
void Shader::setUniforms(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
    GLint modelLoc = glGetUniformLocation(this->Program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    GLint viewLoc = glGetUniformLocation(this->Program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    GLint projLoc = glGetUniformLocation(this->Program, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

//pass uniform values to variables in shader
void Shader::setUniforms(glm::mat4 view, glm::mat4 projection) {
    GLint viewLoc = glGetUniformLocation(this->Program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLint projLoc = glGetUniformLocation(this->Program, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

//pass clip plane value to shader
void Shader::loadClipPlane(glm::vec4 clipPlane) {
    GLint objClipPlaneLoc = glGetUniformLocation(this->Program, "clipPlane");
    glUniform4f(objClipPlaneLoc, clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);
}

void Shader::loadLightVariables(glm::vec3 lightPos, glm::vec3 lightCol) {
    GLint posLoc = glGetUniformLocation(this->Program, "lightPosition");
    glUniform3f(posLoc, lightPos.x, lightPos.y, lightPos.z);
    GLint colourLoc = glGetUniformLocation(this->Program, "lightColour");
    glUniform3f(colourLoc, lightCol.x, lightCol.y, lightCol.z);
}