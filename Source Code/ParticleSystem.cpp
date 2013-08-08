#include "ParticleSystem.h"
#include <boost\lexical_cast.hpp>
#include "Classes.h"
#include "ShaderManager.h"
#include "ParticleManager.h"
#include "ParticleTexture.h"
#include "Attractor.h"
#include "GLMAttractorUpdate.h"
#include "GLFWWindow.h"
#include "Timer.h"
#include "Particle.h"
#include "FreeLookCamera.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\matrix_access.hpp>
#include <glm\gtc\type_ptr.hpp>

ParticleSystem::ParticleSystem(int numParticles, int iniRadius, float quadLength, int maxFPS, float velocityTranslate, float velocityRotate) :
	_numParticles(numParticles),
	_quadLength(quadLength),
	_iniRadius(iniRadius),
	_maxFPS(maxFPS),
	_velocityTranslate(velocityTranslate),
	_velocityRotate(velocityRotate),
	_useGravity(false),
	_camera(new Camera<glm::mat4, glm::vec4, float>(new FreeLookCamera())),
	_input(new GLFWInput()),
	_attractor(new Attractor<glm::mat4, glm::vec4, float>(NULL)),
	_particleManager(new ParticleManager()),
	_particleTexture(new ParticleTexture()),
	_shaderManager(new ShaderManager())
	{
	
	}

ParticleSystem::~ParticleSystem(){
		deleteParticleSystem();
}

void ParticleSystem::initialize(int width, int height){
	//Initiliaze shader and shader program
	_shaderManager->loadShader(".//vs.glsl", "vertexShader", GL_VERTEX_SHADER);
	_shaderManager->loadShader(".//gs.glsl", "geometryShader", GL_GEOMETRY_SHADER);
	_shaderManager->loadShader(".//fs.glsl", "fragmentShader", GL_FRAGMENT_SHADER);
	_shaderManager->loadShader(".//cs.glsl", "computeShader", GL_COMPUTE_SHADER);

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

	_particleTexture->loadTexture(".//Particle.tga");

	_projectionMatrix = glm::perspective(45.0f, (float)width/(float)height, 0.01f, 1000.0f);

	GLMAttractorUpdate* attUpdate = new GLMAttractorUpdate();
	attUpdate->initAttractor(width, height, 45.0f, 0.01f);

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

	Timer fpsTimer;

	fpsTimer.startTimer();
	fpsTimer.getFrameTime();

	while(running){

		if(fpsTimer.getFrameTimeWithoutActualisation() > (1.0/static_cast<float>(_maxFPS))){
			frameTimeDiff = fpsTimer.getFrameTime();
			time = fpsTimer.getTime();

			input(frameTimeDiff, wnd);
			_camera->updateCamera();
			_attractor->updateAttractor(_camera.get(), _input.get());
			render(frameTimeDiff, glfwGetTime());

			wnd->swapBuffers();

			/////////////Calculate FPS and draw them in the window title////////////////////
			frameCounter++;
			if(fpsTimer.getTime() > 1.0f){
				fps = frameCounter;
				frameCounter = 0;
				fpsTimer.startTimer();
			}

			if(_showFPS){
				wnd->setWindowTitle(boost::lexical_cast<std::string, int>(fps).c_str());
			}
		}
		/////////////////////////////////////////////////////////////////////////////////
		
		//Exit the program if x-icon is pressed or the esc-key
		if(!_input->isKeyPressed(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED)){
			running = true;
		} else {
			running = false;
		}
	}
}

void ParticleSystem::input(double frameTimeDiff, GLFWWindow* wnd){
	if(_input->isKeyPressed('W')){
		_camera->setAddZPos(_velocityTranslate*(float)frameTimeDiff);
	}

	if(_input->isKeyPressed('S')){
		_camera->setAddZPos(-1*_velocityTranslate*(float)frameTimeDiff);
	}


	if(_input->isKeyPressed('D')){
		_camera->setAddXPos(_velocityTranslate*(float)frameTimeDiff);
	}


	if(_input->isKeyPressed('A')){
		_camera->setAddXPos(-1*_velocityTranslate*(float)frameTimeDiff);
	}

	if(_input->isKeyPressedOnce(GLFW_KEY_TAB)){
		if(_showFPS){
			_showFPS = false;
			wnd->setDefaultWindowTitle();
		} else {
			_showFPS = true;
		}
	}

	glfwPollEvents();
	if(!_input->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)){
		if(_input->getXPosDiff() != 0 && _input->getXPosDiff() >= 0){
			_camera->setAddYaw(-(float)_input->getXPosDiff()*_velocityRotate*(float)frameTimeDiff);
		}
		glfwPollEvents();
		if(_input->getXPosDiff() != 0 && _input->getXPosDiff() <= 0){
			_camera->setAddYaw(-(float)_input->getXPosDiff()*_velocityRotate*(float)frameTimeDiff);
		}
		glfwPollEvents();
		if(_input->getYPosDiff() != 0 && _input->getYPosDiff() >= 0){
			_camera->setAddPitch(-(float)_input->getYPosDiff()*_velocityRotate*(float)frameTimeDiff);
		}
		glfwPollEvents();
		if(_input->getYPosDiff() != 0 && _input->getYPosDiff() <= 0){
			_camera->setAddPitch(-(float)_input->getYPosDiff()*_velocityRotate*(float)frameTimeDiff);

		}

	} else {
		if(_input->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)){
			_attractor->updateAttractor(_camera.get(), _input.get());
			_useGravity = true;
		} else {
			_useGravity = false;
		}
	}

	_input->updateInput();
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
		_useGravity?1.0f:-1.0f); //Uses the last vector-entry to determine wether the attractor or the gravity is used

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

	glBindBuffer(GL_ARRAY_BUFFER, _particleManager->getParticleBufferID());
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_POINTS, 0, _numParticles);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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


