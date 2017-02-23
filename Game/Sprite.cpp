#include "Sprite.h"

#include <string>
#include <memory>
#include <cstdint>

#include <GL/gl3w.h>
#define STB_IMAGE_IMPLEMENTATION //TODO: this doesn't belong here
#include <stb_image.h>
#include <PhysFS++/physfs.hpp>
#include <cpplog/cpplog.hpp>

static cpplog::StdErrLogger logger;

/*
Considerations:
- maybe move the responsibility of loading the texture somewhere else.
*/

Sprite::Sprite(const std::string& fileName) {
    //TODO: modularize image loading?
    //TODO: maybe we should use only C-style along with openGL? (make it less gross)
    PhysFS::ifstream imageStream(fileName);
    std::unique_ptr<uint8_t[]> imageData = std::make_unique<uint8_t[]>(imageStream.length());
    imageStream.read((char *)imageData.get(), imageStream.length());
    
    stbi_set_flip_vertically_on_load(true);

    int x = 0, y = 0, channels = 0;
    uint8_t* data = stbi_load_from_memory(imageData.get(), imageStream.length(), &x, &y, &channels, 0);

    int format;
    switch (channels) {
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            LOG_ERROR(logger) << "Pixel format unsupported. Was " << channels << " channels." << std::endl;
            format = GL_RGB;
            break;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    //TODO: refactor this
    GLfloat vertices[] = {
           //position                   //UV-coordinates
        0.0f,       1.0f * y,   0.0f,      0.0f, 1.0f, //top-left
        0.0f,       0.0f,       0.0f,      0.0f, 0.0f, //bottom-left
        1.0f * x,   1.0f * y,   0.0f,      1.0f, 1.0f, //top-right
        1.0f * x,   0.0f,       0.0f,      1.0f, 0.0f  //bottom-right
    };

    GLuint indices[] = {
        0, 1, 2, // First Triangle
        2, 1, 3  // Second Triangle
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);
    GLuint EBO;
    glGenBuffers(1, &EBO);
    //the VAO is the only buffer id we need to keep.
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
}

void Sprite::Draw() const {
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, texture);
}

Sprite::~Sprite() {
    glDeleteTextures(1, &texture);
}
