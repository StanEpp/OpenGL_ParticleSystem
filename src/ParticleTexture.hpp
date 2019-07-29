#pragma once

#include <GL/gl3w.h>
#include <string>

class ParticleTexture
{
public:
    ParticleTexture() = default;
    ParticleTexture(ParticleTexture&) = delete;
    ParticleTexture(ParticleTexture&&) = delete;
    ParticleTexture& operator=(ParticleTexture&) = delete;
    ParticleTexture& operator=(ParticleTexture&&) = delete;
    ~ParticleTexture();

    void loadTexture(const std::string& path);
    void useTexture(GLuint shaderProgramID);

private:
    void deleteTexture() noexcept;

    GLuint _textureID = 0;
};