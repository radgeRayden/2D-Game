#include "Shader.h"

#include <fstream>
#include <iostream>

#include <GL/gl3w.h>
#include <cpplog/cpplog.hpp>
#include <PhysFS++/physfs.hpp>

GLuint Shader::ProgramID() {
    return this->programID;
}

bool Shader::Success() {
    return this->success;
}

Shader::Shader(std::string vertexShaderFilename, std::string fragmentShaderFilename) {
    using namespace std;
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    if (vertexShaderFilename == "") {
        glShaderSource(vertexShader, 1, &defaultVertexSource, 0);
    }
    else {
        GLchar* vertexShaderSource = readDataFromFile(vertexShaderFilename);
        if (vertexShaderSource) {
            glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
            delete vertexShaderSource;
        }
    }

    if (fragmentShaderFilename == "") {
        glShaderSource(fragmentShader, 1, &defaultFragmentSource, 0);
    }
    else {
        GLchar* fragmentShaderSource = readDataFromFile(fragmentShaderFilename);
        if (fragmentShaderSource) {
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
            delete fragmentShaderSource;
        }
    }

    GLint success;
    GLchar infoLog[512] {};

    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        LOG_ERROR(logger) << "Shader compile error: \n" << infoLog << endl;
        this->success = false;
        return;
    }

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        LOG_ERROR(logger) << "Shader compile error: \n" << infoLog << endl;
        this->success = false;
        return;
    }

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        LOG_ERROR(logger) << "Program linking error: \n" << infoLog << endl;
        this->success = false;
        return;
    }

    //TODO: make sure these get cleaned correctly even if the shader compilation fails midway.
    glDetachShader(programID, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(programID, fragmentShader);
    glDeleteShader(fragmentShader);
}

void Shader::Use() {
    glUseProgram(this->programID);
}

GLchar* Shader::readDataFromFile(std::string fileName) {
    if (PhysFS::exists(fileName)) {
        PhysFS::ifstream file(fileName);
        GLchar* data = new GLchar[(size_t) file.length() + 1] { 0 };
        file.read(data, file.length());
        return data;
    }
    else {
        LOG_ERROR(logger) << "Could not open " << fileName << "." << std::endl;
        return nullptr;
    }
}

GLchar* Shader::defaultFragmentSource = 
"#version 330 core\n\
out vec4 color;\n\
in vec4 vertexColor;\n\
in vec2 TexCoord;\n\
uniform sampler2D ourTexture;\n\
void main() {\n\
    color = texture(ourTexture, TexCoord);\n\
}";

GLchar* Shader::defaultVertexSource =
"#version 330 core\n\
layout(location = 0) in vec3 position;\n\
layout(location = 1) in vec2 texCoord;\n\
out vec4 vertexColor;\n\
out vec2 TexCoord;\n\
uniform mat4 model;\n\
uniform mat4 view;\n\
uniform mat4 projection;\n\
void main() {\n\
    gl_Position = projection * view * model * vec4(position, 1.0);\n\
    vertexColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n\
    TexCoord = texCoord;\n\
}";

Shader::~Shader() {
}
