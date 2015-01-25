#include "ParticleTexture.h"

void ParticleTexture::loadTexture(const std::string& path){
	tTGA	tga;
	if (!load_TGA(&tga, path.c_str())){
		throw std::runtime_error("ERROR: Could not load particle texture!");
	}

	glGetError();

	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(
			GL_TEXTURE_2D, 
			0, 
			(tga.alpha) ? (GL_RGB) : (GL_RGB8), 
			tga.width, 
			tga.height, 
			0, 
			(tga.alpha) ? (GL_RGBA) : (GL_RGB), 
			GL_UNSIGNED_BYTE, 
			tga.data
			);

	if(glGetError() != GL_NO_ERROR){
		glDeleteTextures(1, &_textureID);
		free_TGA(&tga);

		throw std::runtime_error("ERROR: Could not initialize particle texture!");
	}
	
}

void ParticleTexture::useTexture(GLuint shaderProgramID){
	glGetError();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureID);

	_uniTexture = glGetUniformLocation(shaderProgramID, "texture");

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error("ERROR: Could not get texture-uniform-location!");
	}
	glUniform1i(_uniTexture, 0);
}

void ParticleTexture::deleteTexture(){
	glDeleteTextures(1, &_textureID);
}