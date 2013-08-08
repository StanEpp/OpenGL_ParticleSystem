#ifndef _PARTICLETEXTURE_
#define _PARTICLETEXTURE_

#ifndef _GLEW_
#define _GLEW_
	#include <GL\glew.h>
	#include <GL\glfw.h>
#endif

class ParticleTexture{
private:
	GLuint	_textureID;
	GLint	_uniTexture;

	void deleteTexture();

public:
	ParticleTexture(){}
	~ParticleTexture(){
		deleteTexture();
	}

void loadTexture(const char* filepath);
void useTexture(GLuint shaderProgramID);

};

#endif