#pragma once
#include <string>
#include <GL/gl3w.h>
#include <cpplog/cpplog.hpp>

class Shader {
public:
    Shader(std::string vertexShaderFilename = "", std::string fragmentShaderFileName = "");
    GLuint ProgramID();
    bool Success();
    void Use();
    ~Shader();
private:
    cpplog::StdErrLogger logger;
    GLchar* readDataFromFile(std::string fileName);
    void Shader::deleteShaderStages(GLuint vertexShader, GLuint fragmentShader);
    static GLchar* defaultVertexSource;
    static GLchar* defaultFragmentSource;
    GLuint programID;
    bool vertexCompilationSuccesful = false;
    bool fragmentCompilationSuccessful = false;
    bool shaderLinkingSuccessful = false;
};