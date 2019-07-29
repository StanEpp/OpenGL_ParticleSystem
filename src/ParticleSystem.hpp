#pragma once

#include <GL/gl3w.h>

#include "ShaderManager.hpp"
#include "ParticleBuffer.hpp"
#include "ParticleTexture.hpp"
#include "Attractor.hpp"
#include "GLFWWindow.hpp"
#include "GLFWInput.hpp"
#include "Camera.hpp"
#include "ConfigLoader.hpp"

class ParticleSystem
{
private:
    Settings m_settings;
    GLFWWindow m_window;
    Camera m_camera;
    Attractor m_attractor;
    GLFWInput m_inputControl;
    ParticleBuffer m_particleBuffer;
    ParticleTexture m_particleTexture;
    GLuint m_vertexUVBufferID = 0;
    GLuint m_vertexArrayID = 0;
    bool m_showFPS = false;
    GLuint m_computeProgID = 0;
    GLuint m_shaderProgID = 0;

    ShaderManager m_shaderManager;

    struct ComputeShaderUniformLocations {
        GLuint dt = 0;
        GLuint attPos = 0;
    } m_csLocations;

    struct ParticleShaderUniformLocations {
        GLuint viewMatrix = 0;
        GLuint camPos = 0;
        GLuint time = 0;
    } m_psLocations;

    void render(double);
    void updateParticles(double);

  public:
    ParticleSystem() = delete;
    ParticleSystem(const std::string&);
    ParticleSystem(ParticleSystem&) = delete;
    ParticleSystem(ParticleSystem&&) = delete;
    ParticleSystem& operator=(ParticleSystem&) = delete;
    ParticleSystem& operator=(ParticleSystem&&) = delete;
    ~ParticleSystem();

    void initialize();
    void run();
};