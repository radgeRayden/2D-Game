#include "Sprite.h"
#include <string>
#include <memory>
#include <cstdint>
#include <GL/gl3w.h>
#define STB_IMAGE_IMPLEMENTATION //TODO: this doesn't belong here
#include <stb_image.h>
#include <PhysFS++/physfs.hpp>

Sprite::Sprite(const std::string& fileName) {
    //TODO: refactor this
    GLfloat vertices[] = {
        //position            //UV-coordinates
        0.0f,   0.0f,   0.0f, 0.0f, 0.0f,      //top-left
        100.0f, 0.0f,   0.0f, 1.0f, 0.0f,      //top-right
        100.0f, 100.0f, 0.0f, 1.0f, 1.0f,      //bottom-right
        0.0f,   100.0f, 0.0f, 0.0f, 1.0f       //bottom-left
    };

    GLuint indices[] = {
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);
    GLuint EBO;
    glGenBuffers(1, &EBO);
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        //position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid *) 0);
        glEnableVertexAttribArray(0);

        //UV coordinates
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid *) (3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    //TODO: modularize image loading?
    PhysFS::ifstream imageStream(fileName);
    //TODO: maybe we should use only C-style along with openGL?
    std::unique_ptr<uint8_t> imageData = std::make_unique<uint8_t>(imageStream.length());
    imageStream.read((char *)imageData.get(), imageStream.length()); //:thinking:

    stbi_set_flip_vertically_on_load(true);

    int x = 0, y = 0, n = 0;
    uint8_t* data = stbi_load_from_memory(imageData.get(), imageStream.length(), &x, &y,&n, 0);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

void Sprite::Draw() const {
    
}

Sprite::~Sprite() {
}
