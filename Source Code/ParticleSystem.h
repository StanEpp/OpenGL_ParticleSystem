#ifndef _PARTICLESYSTEM_
#define _PARTICLESYSTEM_

#define WORK_GROUP_SIZE 256

#include <boost\shared_ptr.hpp>
#include <glm\glm.hpp>
#include "Classes.h"
#include "Camera.h"
#include "Attractor.h"


class ParticleSystem{
private:
	boost::shared_ptr<Camera<glm::mat4, glm::vec4, float> >		_camera;
	boost::shared_ptr<Input>									_input;
	boost::shared_ptr<Attractor<glm::mat4, glm::vec4, float> >	_attractor;
	boost::shared_ptr<ParticleManager>							_particleManager;
	boost::shared_ptr<ParticleTexture>							_particleTexture;
	boost::shared_ptr<ShaderManager>							_shaderManager;
	glm::mat4													_projectionMatrix;
	int															_numParticles, _iniRadius, _maxFPS;
	float														_quadLength, _velocityTranslate, _velocityRotate;
	bool														_useGravity, _showFPS;

	void initialize(int width, int height);
	void render(double frameTimeDiff, double time);
	void input(double frameTimeDiff, GLFWWindow* wnd);
	void deleteParticleSystem();

public:
	ParticleSystem(int numParticles, int iniRadius, float _quadLength, int maxFPS, float velocityTranslate, float velocityRotate);
	~ParticleSystem();

	void run(GLFWWindow* wnd);
	void resize(int width, int height);


};



#endif