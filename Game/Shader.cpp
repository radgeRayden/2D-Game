#include "Shader.h"

#include <fstream>
#include <iostream>

#include <GL/gl3w.h>
#include <cpplog/cpplog.hpp>
#include <PhysFS++/physfs.hpp>

GLuint Shader::ProgramID() const {
    return programID;
}

bool Shader::Success() const {
    return vertexCompilationSuccessful && fragmentCompilationSuccessful && shaderLinkingSuccessful;
}

Shader::Shader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    /*
        We need to make sure of a few things here:
        1 - there's a filename provided; if not, use the default shaders (for each non provided name);
        2 - if given a name, the file can be properly loaded before sending the shader for compilation.
        Manual checking is necessary since OpenGL requires a raw pointer to a C string.
    */
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

    //compile shader, emit error if reported, set success flags (x3).
    GLint success = 0;
    GLchar infoLog[512] {};
    
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        LOG_ERROR(logger) << "Shader compile error: \n" << infoLog << std::endl;
        deleteShaderStages(vertexShader, fragmentShader);
        return;
    }
    vertexCompilationSuccessful = true;

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        LOG_ERROR(logger) << "Shader compile error: \n" << infoLog << std::endl;
        deleteShaderStages(vertexShader, fragmentShader);
        return;
    }
    fragmentCompilationSuccessful = true;

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        LOG_ERROR(logger) << "Program linking error: \n" << infoLog << std::endl;
        deleteShaderStages(vertexShader, fragmentShader);
        return;
    }
    shaderLinkingSuccessful = true;

    deleteShaderStages(vertexShader, fragmentShader);
}

void Shader::Use() const {
    glUseProgram(programID);
}

GLchar* Shader::readDataFromFile(const std::string& fileName) {
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

//deletes only the shader stages that didn't fail to compile
void Shader::deleteShaderStages(GLuint vertexShader, GLuint fragmentShader) {
    if (vertexCompilationSuccessful) {
        glDetachShader(programID, vertexShader);
        glDeleteShader(vertexShader);
    }
    if (fragmentCompilationSuccessful) {
        glDetachShader(programID, fragmentShader);
        glDeleteShader(fragmentShader);
    }
}

//curly braces so I can collapse the strings under the IDE.

const GLchar* Shader::defaultFragmentSource = {
                    "#version 330 core\n\
                    in vec2 TexCoord;\n\
                    out vec4 color;\n\
                    uniform sampler2D boundTexture;\n\
                    void main() {\n\
                        vec4 texColor = texture(boundTexture, TexCoord);\n\
                        if (texColor.a < 0.1)\n\
                            discard;\n\
                        color = texColor;\n\
                    }" 
};

const GLchar* Shader::defaultVertexSource = {
                    "#version 330 core\n\
                    layout(location = 0) in vec3 position;\n\
                    layout(location = 1) in vec2 texCoord;\n\
                    out vec2 TexCoord;\n\
                    uniform mat4 transform;\n\
                    void main() {\n\
                        gl_Position = transform * vec4(position, 1.0);\n\
                        TexCoord = texCoord;\n\
                    }" 
};

Shader::~Shader() {
    glDeleteProgram(programID);
}
