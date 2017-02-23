#pragma once
#include <string>
#include <GL/gl3w.h>

class Sprite {
public:
    Sprite(const std::string& fileName);
    void Draw() const;
    ~Sprite();
private:
    GLuint VAO;
    GLuint texture;
};

