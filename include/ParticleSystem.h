#ifndef _PARTICLESYSTEM_
#define _PARTICLESYSTEM_

#ifndef _GLEW_
#define _GLEW_
	#include <GL\glew.h>
	#include <GLFW\glfw3.h>
#endif

#define WORK_GROUP_SIZE 256
#define GLM_FORCE_RADIANS

#include <iostream>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\matrix_access.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <memory>
#include <boost\lexical_cast.hpp>

#include "Classes.h"
#include "ShaderManager.h"
#include "ParticleManager.h"
#include "ParticleTexture.h"
#include "Attractor.h"
#include "RayCastAttractorUpdate.h"
#include "IAttractorUpdate.h"
#include "GLFWWindow.h"
#include "GLFWInput.h"
#include "GLFWTimer.h"
#include "Particle.h"
#include "FPCamera.h"
#include "Camera.h"
#include "FPCamera.h"

struct Vertex{
	glm::vec4 pos;
};

class ParticleSystem{
private:
	std::shared_ptr<Camera<glm::mat4, glm::vec4, float> >	_camera;
	std::shared_ptr<GLFWInput>	_input;
	std::shared_ptr<Attractor<glm::mat4, glm::vec4, float> >	_attractor;
	std::shared_ptr<ParticleManager>	_particleManager;
	std::shared_ptr<ParticleTexture>	_particleTexture;
	glm::mat4	_projectionMatrix;
	int	_numParticles, _iniRadius, _maxFPS;
	float	_quadLength, _velocityTranslate, _velocityRotate;
	bool	_useGravity, _showFPS;

	ShaderManager*	_shaderManager;

	void initialize(int width, int height);
	void render(double frameTimeDiff, double time);
	void input(double frameTimeDiff, GLFWWindow* wnd);
	void deleteParticleSystem() noexcept;

	GLuint	_vertexUVBufferID;
	GLuint 	_VertexArrayID;
public:
	ParticleSystem(int numParticles, int iniRadius, float _quadLength, int maxFPS, float velocityTranslate, float velocityRotate);
	~ParticleSystem();

	void run(GLFWWindow* wnd);
	void resize(int width, int height);


};


#endif