#pragma once

#include <GL/gl3w.h>
#include <glm/glm.hpp>

struct Particle
{
    glm::vec4 currPosition;
    glm::vec4 prevPosition;
};

class ParticleBuffer
{
public:
    ParticleBuffer() = default;
    ParticleBuffer(unsigned int, int);
    ParticleBuffer(ParticleBuffer&) = delete;
    ParticleBuffer(ParticleBuffer&&) = delete;
    ParticleBuffer& operator=(ParticleBuffer&) = delete;
    ParticleBuffer& operator=(ParticleBuffer&&) = delete;
    ~ParticleBuffer();

    void initializeParticles();

    GLuint getParticleBufferID() const;
    unsigned int getNumParticles() const;

private:
    void distributeParticles(Particle*);
    void deleteParticleBuffer() noexcept;
    GLuint _buffID = 0;
    unsigned int _numParticles = 1000;
    int _initRadius = 15;
};