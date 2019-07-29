#include "ParticleSystem.hpp"

#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLFWTimer.hpp"

#define WORK_GROUP_SIZE 256

ParticleSystem::ParticleSystem(const std::string& settingsFile) :
    m_settings(ConfigLoader(settingsFile).settings()),
    m_window(m_settings.width, m_settings.height, "OpenGL_4.3_Particle_System", m_settings.fullscreen),
    m_camera(m_settings.width, m_settings.height, m_settings.fovY,
            glm::vec3(0, 0, -5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0),
            m_settings.nearPlane, m_settings.farPlane,
            m_settings.cameraSensitivity, m_settings.movementSpeed),
    m_particleBuffer(m_settings.numParticles, m_settings.particleSpawnRadius),
    m_showFPS(false)
{
   m_inputControl.bindInputToWindow(m_window);
}

ParticleSystem::~ParticleSystem()
{
    if (m_computeProgID) {
        m_shaderManager.deleteProgram(m_computeProgID);
    }

    if (m_shaderProgID) {
        m_shaderManager.deleteProgram(m_shaderProgID);
    }
}

void ParticleSystem::initialize()
{
    namespace fs = std::filesystem;

    //////// Initialize GLFW window, input, rendering context and gl3w
    m_window.setVSync(false);
    m_inputControl.bindInputToWindow(m_window);

    if (gl3wInit()) throw std::runtime_error("Could not initialize gl3w!");
    if (!gl3wIsSupported(4, 3)) throw std::runtime_error("OpenGL 4.3 not supported!");

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Generate Vertex Arrays and initialize particles
    m_particleBuffer.initializeParticles();

    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_particleBuffer.getParticleBufferID());
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer.getParticleBufferID());
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)0);
    glBindVertexArray(0);

    //Initiliaze shader and shader program
    fs::path shaderFolder("shader");
    m_shaderManager.loadShader(shaderFolder/"vs.glsl", "vertexShader", GL_VERTEX_SHADER);
    m_shaderManager.loadShader(shaderFolder/"gs.glsl", "geometryShader", GL_GEOMETRY_SHADER);
    m_shaderManager.loadShader(shaderFolder/"fs.glsl", "fragmentShader", GL_FRAGMENT_SHADER);
    m_shaderManager.loadShader(shaderFolder/"cs.glsl", "computeShader", GL_COMPUTE_SHADER);

    m_shaderProgID = m_shaderManager.createProgram("shaderProg");
    m_computeProgID = m_shaderManager.createProgram("computeProg");

    m_shaderManager.attachShader("vertexShader", "shaderProg");
    m_shaderManager.attachShader("geometryShader", "shaderProg");
    m_shaderManager.attachShader("fragmentShader", "shaderProg");
    m_shaderManager.attachShader("computeShader", "computeProg");

    m_shaderManager.linkProgram("computeProg");
    m_shaderManager.linkProgram("shaderProg");

    // Since the programs are linked, the shaders are not needed anymore
    m_shaderManager.deleteShader("vertexShader");
    m_shaderManager.deleteShader("geometryShader");
    m_shaderManager.deleteShader("fragmentShader");
    m_shaderManager.deleteShader("computeShader");

    m_particleTexture.loadTexture(fs::path("texture")/"Particle.tga");

    // Retrieve the uniform locations for both shaders and set the
    // uniform variables which will not change for every frame.
    m_shaderManager.useProgram(m_computeProgID);
    m_shaderManager.loadUniform_(m_computeProgID, "maxParticles", m_particleBuffer.getNumParticles());
    m_csLocations.dt = m_shaderManager.getUniformLocation(m_computeProgID, "dt");
    m_csLocations.attPos = m_shaderManager.getUniformLocation(m_computeProgID, "attPos");

    m_shaderManager.useProgram(m_shaderProgID);
    m_shaderManager.loadUniform_(m_shaderProgID, "quadLength", m_settings.sizeOfParticle);
    m_shaderManager.loadMatrix4(m_shaderProgID, "projMatrix", glm::value_ptr(m_camera.projectionMatrix()));
    m_psLocations.viewMatrix = m_shaderManager.getUniformLocation(m_shaderProgID, "viewMatrix");
    m_psLocations.camPos = m_shaderManager.getUniformLocation(m_shaderProgID, "camPos");
    m_psLocations.time = m_shaderManager.getUniformLocation(m_shaderProgID, "time");

    m_shaderManager.resetProgram();

    // Depth test needs to be disabled for only rendering transparent particles.
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

void ParticleSystem::run()
{
    bool running = true;

    GLFWTimer mainTimer;
    GLFWTimer fpsTimer;

    std::string fpsStr;
    uint32_t frameCounter = 0;
    double dt = 0.;

    while (running) {
        // TODO: too high fps changes physics behavior.
        // Artificially limit max fps to 60.
        if (mainTimer.timestampDiff() > 1./60.) {
            dt = mainTimer.timestampDiff();
            mainTimer.setTimestamp();

            // Exit the program if ESC is pressed
            if (m_inputControl.isKeyPressed(GLFW_KEY_ESCAPE)) {
                running = false;
            }
            // Show FPS when TAB is pressed
            if (m_inputControl.isKeyPressedOnce(GLFW_KEY_TAB)) {
                m_showFPS = !m_showFPS;
                if (!m_showFPS) {
                    m_window.setDefaultWindowTitle();
                }
            }

            m_inputControl.updateInput();

            m_camera.update(m_inputControl, dt);
            m_attractor.updateAttractor(m_camera, m_inputControl);

            updateParticles(dt);
            render(mainTimer.timestamp());

            // Compute FPS and store them in a string.
            if (fpsTimer.timestampDiff() > 1.0) {
                if (m_showFPS) {
                    m_window.setWindowTitle(std::to_string(frameCounter).c_str());
                }
                frameCounter = 0;
                fpsTimer.setTimestamp();
            }

            m_window.swapBuffers();
            frameCounter++;
        }
    }
}

void ParticleSystem::updateParticles(double dt)
{
    m_shaderManager.useProgram(m_computeProgID);

    m_shaderManager.loadUniform(m_csLocations.dt, static_cast<GLfloat>(dt));

    m_shaderManager.loadUniform(m_csLocations.attPos,
    m_attractor.pos().x,
    m_attractor.pos().y,
    m_attractor.pos().z,
    m_attractor.active() ? 1.0f : -1.0f); //Uses the last vector-entry to determine whether attractor or gravity is used

    glDispatchCompute((m_particleBuffer.getNumParticles()/WORK_GROUP_SIZE) + 1, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
}

void ParticleSystem::render(double time)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(m_vertexArrayID);

    m_shaderManager.useProgram(m_shaderProgID);

    m_shaderManager.loadMatrix4(m_psLocations.viewMatrix, glm::value_ptr(m_camera.viewMatrix()));
    m_shaderManager.loadUniform(m_psLocations.camPos,
      m_camera.pos().x,
      m_camera.pos().y,
      m_camera.pos().z,
      1.0f);

    m_shaderManager.loadUniform(m_psLocations.time, static_cast<GLfloat>(time));

    glDrawArrays(GL_POINTS, 0, m_particleBuffer.getNumParticles());

    glBindVertexArray(0);

    m_shaderManager.resetProgram();
}