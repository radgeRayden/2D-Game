#pragma once
#include <string>
#include <GL/gl3w.h>
#include <cpplog/cpplog.hpp>

class Shader {
public:
    Shader(const std::string& vertexShaderFilename = "", const std::string& fragmentShaderFileName = "");
    GLuint ProgramID() const;
    bool Success() const;
    void Use() const;
    ~Shader();
private:
    cpplog::StdErrLogger logger;
    GLchar* readDataFromFile(const std::string& fileName);
    void Shader::deleteShaderStages(GLuint vertexShader, GLuint fragmentShader);
    static GLchar* defaultVertexSource;
    static GLchar* defaultFragmentSource;
    GLuint programID = 0;
    bool vertexCompilationSuccessful = false;
    bool fragmentCompilationSuccessful = false;
    bool shaderLinkingSuccessful = false;
};