#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int numParticles, int iniRadius, float quadLength, int maxFPS, float velocityTranslate, float velocityRotate) :
	_numParticles(numParticles),
	_quadLength(quadLength),
	_iniRadius(iniRadius),
	_maxFPS(maxFPS),
	_velocityTranslate(velocityTranslate),
	_velocityRotate(velocityRotate),
	_useGravity(false),
	_camera(new Camera<glm::mat4, glm::vec4, float>(new FPCamera())),
	_input(new GLFWInput()),
	_attractor(new Attractor<glm::mat4, glm::vec4, float>(NULL)),
	_particleManager(new ParticleManager()),
	_particleTexture(new ParticleTexture()),
	_shaderManager(ShaderManager::getInstance())
	{
	
	}

ParticleSystem::~ParticleSystem(){
		deleteParticleSystem();
}

void ParticleSystem::initialize(int width, int height){
	glGenVertexArrays(1, &_VertexArrayID);
	glBindVertexArray(_VertexArrayID);
	
	//Initiliaze shader and shader program
	_shaderManager->loadShader("vs.glsl", "vertexShader", GL_VERTEX_SHADER);
	_shaderManager->loadShader("gs.glsl", "geometryShader", GL_GEOMETRY_SHADER);
	_shaderManager->loadShader("fs.glsl", "fragmentShader", GL_FRAGMENT_SHADER);
	_shaderManager->loadShader("cs.glsl", "computeShader", GL_COMPUTE_SHADER);

	_shaderManager->createProgram("shaderProg");
	_shaderManager->createProgram("computeProg");

	_shaderManager->attachShader("vertexShader", "shaderProg");
	_shaderManager->attachShader("geometryShader", "shaderProg");
	_shaderManager->attachShader("fragmentShader", "shaderProg");
	_shaderManager->attachShader("computeShader", "computeProg");

	_shaderManager->linkProgram("computeProg");
	_shaderManager->linkProgram("shaderProg");

	//Since the programs are linked the shaders are not needed anymore
	_shaderManager->deleteShader("vertexShader");
	_shaderManager->deleteShader("geometryShader");
	_shaderManager->deleteShader("fragmentShader");
	_shaderManager->deleteShader("computeShader");

	_particleManager->loadParticleBuffer(_numParticles, _iniRadius);

	_particleTexture->loadTexture("Particle.tga");

	_projectionMatrix = glm::perspective(45.0f, (float)width/(float)height, 1.f, 1000.0f);

	boost::shared_ptr<RayCastAttractorUpdate> attUpdate(new RayCastAttractorUpdate());
	attUpdate->initAttractor(width, height, 45.0f, 1.f);
	_attractor->setUpdateStrategy(attUpdate);

	_camera->setPosition(glm::vec4(0,0,0,1));
	_camera->setAddPitch(0); _camera->setAddRoll(0); _camera->setAddYaw(0);
	_camera->setAddXPos(0); _camera->setAddYPos(0); _camera->setAddZPos(0);
	_camera->setPitch(0); _camera->setRoll(0); _camera->setYaw(0);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

} 

void ParticleSystem::resize(int width, int height){
	_projectionMatrix = glm::perspective(45.0f, (float)width/(float)height, 0.01f, 1000.0f);
}

void ParticleSystem::run(GLFWWindow* wnd){
	initialize(wnd->getWidth(), wnd->getHeight());

	bool running = true;

	int count = 0, frameCounter = 0, fps = 0;
	double frameTimeDiff, time;

	GLFWTimer fpsTimer;

	fpsTimer.getTimeDiff();

	while(running){

		if(fpsTimer.getTimeDiffWithoutActualization() > (1.0/static_cast<float>(_maxFPS))){
			frameTimeDiff = fpsTimer.getTimeDiff();

			input(frameTimeDiff, wnd);
			_camera->updateCamera();
			_attractor->updateAttractor(_camera.get(), _input.get());
			render(frameTimeDiff, fpsTimer.getRefreshedTime());

			wnd->swapBuffers();

			/////////////Calculate FPS and draw them in the window title////////////////////
			frameCounter++;
			if(fpsTimer.getRefreshedTime() > 1.0f){
				fps = frameCounter;
				frameCounter = 0;
				fpsTimer.resetTime();
			}

			if(_showFPS){
				wnd->setWindowTitle(boost::lexical_cast<std::string, int>(fps).c_str());
			}
		}
		/////////////////////////////////////////////////////////////////////////////////
		
		//Exit the program if the esc-key is pressed
		if(_input->isKeyPressed(GLFW_KEY_ESCAPE, wnd->getGLFWwindow())){
			running = false;
		}
	}
}

void ParticleSystem::render(double frameTimeDiff, double time){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _particleManager->getParticleBufferID());
	_shaderManager->useProgram("computeProg");

	_particleManager->loadFloatUniform(
		_shaderManager->getShaderProgramID("computeProg"),
		"frameTimeDiff",
		static_cast<float>(frameTimeDiff));
	
	_particleManager->loadVec4Uniform(
		_shaderManager->getShaderProgramID("computeProg"), 
		"attPos",
		_attractor->getAttractorPos().x,
		_attractor->getAttractorPos().y,
		_attractor->getAttractorPos().z,
		_useGravity?1.0f:-1.0f); //Uses the last vector-entry to determine whether the attractor or the gravity is used

	_particleManager->loadUintUniform(
		_shaderManager->getShaderProgramID("computeProg"), 
		"maxParticles",
		(GLuint)_numParticles
		);
	
	glDispatchCompute((_numParticles/WORK_GROUP_SIZE)+1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

	_shaderManager->useProgram("shaderProg");

	_particleManager->loadMatrix4Uniform(
		_shaderManager->getShaderProgramID("shaderProg"),
		"viewMatrix",
		glm::value_ptr(_camera->getViewMatrix()));

	_particleManager->loadVec4Uniform(
		_shaderManager->getShaderProgramID("shaderProg"),
		"camPos",
		_camera->getPosition().x,
		_camera->getPosition().y,
		_camera->getPosition().z,
		1.0f);

	_particleManager->loadMatrix4Uniform(
		_shaderManager->getShaderProgramID("shaderProg"),
		"projMatrix",
		glm::value_ptr(_projectionMatrix));

	_particleManager->loadFloatUniform(
		_shaderManager->getShaderProgramID("shaderProg"),
		"quadLength",
		_quadLength);

	_particleManager->loadFloatUniform(
		_shaderManager->getShaderProgramID("shaderProg"),
		"time",
		static_cast<float>(time));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _particleManager->getParticleBufferID());
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)0);
	glDrawArrays(GL_POINTS, 0, _numParticles);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::input(double frameTimeDiff, GLFWWindow* wnd){
	GLFWwindow* glfwnd = wnd->getGLFWwindow();
	
	if(_input->isKeyPressed(GLFW_KEY_W, glfwnd)){
		_camera->setAddZPos(_velocityTranslate*(float)frameTimeDiff);
	}

	if(_input->isKeyPressed(GLFW_KEY_S, glfwnd)){
		_camera->setAddZPos(-1*_velocityTranslate*(float)frameTimeDiff);
	}

	if(_input->isKeyPressed(GLFW_KEY_D, glfwnd)){
		_camera->setAddXPos(_velocityTranslate*(float)frameTimeDiff);
	}

	if(_input->isKeyPressed(GLFW_KEY_A, glfwnd)){
		_camera->setAddXPos(-1*_velocityTranslate*(float)frameTimeDiff);
	}
	
	if(_input->isKeyPressed(GLFW_KEY_UP, glfwnd)){
		_attractor->incrementDepth(1.0f);
	}
	
	if(_input->isKeyPressed(GLFW_KEY_DOWN, glfwnd)){
		_attractor->incrementDepth(-1.0f);
	}

	if(_input->isKeyPressedOnce(GLFW_KEY_TAB, glfwnd)){
		if(_showFPS){
			_showFPS = false;
			wnd->setDefaultWindowTitle();
		} else {
			_showFPS = true;
		}
	}

	if(!_input->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT, glfwnd)){
		glfwPollEvents();
		if(_input->getXPosDiff(glfwnd) != 0 && _input->getXPosDiff(glfwnd) >= 0){
			_camera->setAddYaw(-(float)_input->getXPosDiff(glfwnd)*_velocityRotate*(float)frameTimeDiff);
		}
		glfwPollEvents();
		if(_input->getXPosDiff(glfwnd) != 0 && _input->getXPosDiff(glfwnd) <= 0){
			_camera->setAddYaw(-(float)_input->getXPosDiff(glfwnd)*_velocityRotate*(float)frameTimeDiff);
			
		}
		glfwPollEvents();
		if(_input->getYPosDiff(glfwnd) != 0 && _input->getYPosDiff(glfwnd) >= 0){
			_camera->setAddPitch(-(float)_input->getYPosDiff(glfwnd)*_velocityRotate*(float)frameTimeDiff);
		}
		glfwPollEvents();
		if(_input->getYPosDiff(glfwnd) != 0 && _input->getYPosDiff(glfwnd) <= 0){
			_camera->setAddPitch(-(float)_input->getYPosDiff(glfwnd)*_velocityRotate*(float)frameTimeDiff);

		}

	} else {
		if(_input->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT, glfwnd)){
			_attractor->updateAttractor(_camera.get(), _input.get());
			_useGravity = true;
		} else {
			_useGravity = false;
		}
	}

	_input->updateInput(glfwnd);
}

void ParticleSystem::deleteParticleSystem(){
	glUseProgram(0);

	try{
		_shaderManager->deleteProgram("shaderProg");
		_shaderManager->deleteProgram("computeProg");
	}catch(std::exception& e){
		std::cout << e.what() <<std::endl;
	}

}


