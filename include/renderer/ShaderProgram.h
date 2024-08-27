#pragma once

#include "renderer/LocalGL.h"

#include <string>

namespace perfectpixel { namespace renderer {

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

public:
    void addShader(GLenum shaderType, const std::string &source);
    void link();
    void use();

    GLuint getUniformLocation(const std::string &name);

private:
    GLuint m_id;
    bool m_linked;
};

}} // namespace perfectpixel::renderer