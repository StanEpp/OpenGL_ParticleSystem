#include "ParticleBuffer.hpp"

#include <memory>
#include <iostream>
#include <random>

ParticleBuffer::ParticleBuffer(unsigned int numParticles, int initRadius) :
    _numParticles(numParticles),
    _initRadius(initRadius)
{}

ParticleBuffer::~ParticleBuffer()
{
    deleteParticleBuffer();
}

void ParticleBuffer::initializeParticles()
{
    auto particles = std::make_unique<Particle[]>(_numParticles);
    distributeParticles(particles.get());

    glGetError();

    glGenBuffers(1, &_buffID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _buffID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, _numParticles * sizeof(Particle), particles.get(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _buffID);

    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error("ERROR: Could not generate or bind shader storage buffer for particles!");
    }
}

void ParticleBuffer::distributeParticles(Particle* particles)
{
    float rndX, rndY, rndZ;
    auto fInitRadius = static_cast<float>(_initRadius);
    std::mt19937 eng;
    std::uniform_real_distribution<float> dist(fInitRadius*(-1.f), fInitRadius);

    for (auto i = 0u; i < _numParticles; ++i) {
        rndX = dist(eng);
        rndY = dist(eng);
        rndZ = dist(eng);
        particles[i].currPosition = glm::vec4(rndX, rndY, rndZ, 1.f);
        particles[i].prevPosition = glm::vec4(rndX, rndY, rndZ, 1.f);
    }
}

GLuint ParticleBuffer::getParticleBufferID() const
{
    return _buffID;
}

unsigned int ParticleBuffer::getNumParticles() const
{
    return _numParticles;
}

void ParticleBuffer::deleteParticleBuffer() noexcept
{
    if(!_buffID) return;
    glGetError();
    glDeleteBuffers(1, &_buffID);
    if (glGetError() != GL_NO_ERROR) {
        std::cerr << "Could not properly delete particle Buffer!" << std::endl;
    }
    _buffID = 0;
}