#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem(const Config& config) :
  _window(config.window.width, config.window.height, "OpenGL_4.3_Particle_System", config.window.fullscreen),
  _input(),
  _camera(config.camera.speed, config.camera.sensitivity, config.camera.foV, config.window.width, config.window.height, config.camera.nearDist, config.camera.farDist),
  _attractor(),
  _particleBuffer(config.particles.numParticles, config.particles.initRadius),
  _particleTexture(),
  _quadLength(config.particles.sizeOfParticles),
  _showFPS(false),
  _computeProgID(0), _shaderProgID(0),
  _shaderManager()
  {}

ParticleSystem::~ParticleSystem(){
  deleteParticleSystem();
}

void ParticleSystem::initialize(){
  //////// Initialize GLFW window, input, rendering context and gl3w
  _window.initialize();
  _window.setVSync(true);
  _input.bindInputToWindow(_window);
  
  if(gl3wInit()) throw std::runtime_error("Could not initialize gl3w!");
  if(!gl3wIsSupported(4, 3)) throw std::runtime_error("OpenGL 4.3 not supported!");
  
  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  
  // Generate Vertex Arrays and initialize particles
  _particleBuffer.initializeParticles();
  
  glGenVertexArrays(1, &_vertexArrayID);
  glBindVertexArray(_vertexArrayID);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _particleBuffer.getParticleBufferID());
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _particleBuffer.getParticleBufferID());
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)0);
  glBindVertexArray(0);

  //Initiliaze shader and shader program
  _shaderManager.loadShader("vs.glsl", "vertexShader", GL_VERTEX_SHADER);
  _shaderManager.loadShader("gs.glsl", "geometryShader", GL_GEOMETRY_SHADER);
  _shaderManager.loadShader("fs.glsl", "fragmentShader", GL_FRAGMENT_SHADER);
  _shaderManager.loadShader("cs.glsl", "computeShader", GL_COMPUTE_SHADER);

  _shaderProgID = _shaderManager.createProgram("shaderProg");
  _computeProgID = _shaderManager.createProgram("computeProg");

  _shaderManager.attachShader("vertexShader", "shaderProg");
  _shaderManager.attachShader("geometryShader", "shaderProg");
  _shaderManager.attachShader("fragmentShader", "shaderProg");
  _shaderManager.attachShader("computeShader", "computeProg");

  _shaderManager.linkProgram("computeProg");
  _shaderManager.linkProgram("shaderProg");

  // Since the programs are linked, the shaders are not needed anymore
  _shaderManager.deleteShader("vertexShader");
  _shaderManager.deleteShader("geometryShader");
  _shaderManager.deleteShader("fragmentShader");
  _shaderManager.deleteShader("computeShader");

  _particleTexture.loadTexture("Particle.tga");

  _camera.setPosition(glm::vec4(0,0,0,1));
  
  // Retrieve the uniform locations for both shaders and set the 
  // uniform variables which will not change for every frame.
  _shaderManager.useProgram(_computeProgID);
  _shaderManager.loadUniform_(_computeProgID, "maxParticles", _particleBuffer.getNumParticles());
  _csLocations.frameTimeDiff = _shaderManager.getUniformLocation(_computeProgID, "frameTimeDiff");
  _csLocations.attPos = _shaderManager.getUniformLocation(_computeProgID, "attPos");
   
  _shaderManager.useProgram(_shaderProgID);
  _shaderManager.loadUniform_(_shaderProgID, "quadLength", _quadLength);
  _shaderManager.loadMatrix4(_shaderProgID, "projMatrix", glm::value_ptr(_camera.getProjectionMatrix()));
  _psLocations.viewMatrix = _shaderManager.getUniformLocation(_shaderProgID, "viewMatrix");
  _psLocations.camPos = _shaderManager.getUniformLocation(_shaderProgID, "camPos");
  _psLocations.time = _shaderManager.getUniformLocation(_shaderProgID, "time");
  
  _shaderManager.resetProgram();

  // Depth test needs to be disabled for only rendering transparent particles. 
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

void ParticleSystem::resize(int width, int height){
  _camera.resize(width, height);
  _shaderManager.useProgram(_shaderProgID);
  _shaderManager.loadMatrix4(_shaderProgID, "projMatrix", glm::value_ptr(_camera.getProjectionMatrix()));
  _shaderManager.resetProgram();
}

void ParticleSystem::run(){
  
  using namespace std::chrono;
  
  bool running = true;
  
  Timer timer;
  timer.start = Timer::currentTime();
  timer.nextGameTick = Timer::currentTime();
  timer.lastFpsUpdate = timer.nextGameTick;
  timer.lastFrameRendered = timer.nextGameTick;
  std::string fpsStr;
  
  while(running){
    timer.skippedFrames = 0;
    
    while(Timer::currentTime() > timer.nextGameTick && timer.skippedFrames < timer.maxFrameskip){
      _input.updateInput();
      
      // Exit the program if ESC is pressed
      if(_input.isKeyPressed(GLFW_KEY_ESCAPE)){
        running = false;
      }
      // Show FPS when TAB is pressed
      if(_input.isKeyPressedOnce(GLFW_KEY_TAB)){
        _showFPS = !_showFPS;
        if(!_showFPS) _window.setDefaultWindowTitle();
      }
      // Enable/Disable VSync when Space is pressed
      if(_input.isKeyPressedOnce(GLFW_KEY_SPACE)){
        _window.setVSync(!_window.isVSyncOn());
      }
      timer.nextGameTick += timer.skipTicks;
      timer.skippedFrames++;
    }
    
    auto dt = duration_cast<duration<double>>(Timer::currentTime() - timer.lastFrameRendered).count();
    timer.lastFrameRendered = Timer::currentTime();
    
    _camera.updateCamera(dt, _input);
    _attractor.updateAttractor(_camera, _input);
    auto currTime = duration_cast<milliseconds>(timer.start - Timer::currentTime()).count();
    render(dt, currTime);

    // Compute FPS and store them in a string.
    if (duration_cast<milliseconds>(Timer::currentTime() - timer.lastFpsUpdate) >= milliseconds(1000)) {
      fpsStr = std::to_string(timer.framesRendered);
      timer.lastFpsUpdate = Timer::currentTime();
      timer.framesRendered = 0;
    }
    timer.framesRendered++;

    if(_showFPS) _window.setWindowTitle(fpsStr);
    
    _window.swapBuffers();
  }
}

void ParticleSystem::render(double dt, double time){
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindVertexArray(_vertexArrayID);
  
  _shaderManager.useProgram(_computeProgID);
  
  _shaderManager.loadUniform(_csLocations.frameTimeDiff, static_cast<GLfloat>(dt));
  
  _shaderManager.loadUniform(_csLocations.attPos,
    _attractor.getPosition().x,
    _attractor.getPosition().y,
    _attractor.getPosition().z,
    _attractor.isActive()?1.0f:-1.0f); //Uses the last vector-entry to determine whether the attractor or the gravity is used

  glDispatchCompute((_particleBuffer.getNumParticles()/WORK_GROUP_SIZE)+1, 1, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

  _shaderManager.useProgram(_shaderProgID);

  _shaderManager.loadMatrix4(_psLocations.viewMatrix, glm::value_ptr(_camera.getViewMatrix()));
  _shaderManager.loadUniform(_psLocations.camPos,
    _camera.getPosition().x,
    _camera.getPosition().y,
    _camera.getPosition().z,
    1.0f);

  _shaderManager.loadUniform(_psLocations.time, static_cast<GLfloat>(time));

  glDrawArrays(GL_POINTS, 0, _particleBuffer.getNumParticles());
  
  glBindVertexArray(0);
  
  _shaderManager.resetProgram();
}

void ParticleSystem::deleteParticleSystem() noexcept{
  if(_computeProgID)_shaderManager.deleteProgram(_computeProgID);
  if(_shaderProgID) _shaderManager.deleteProgram(_shaderProgID);
}