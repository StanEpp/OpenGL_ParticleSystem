#include "ParticleTexture.h"

#ifndef _GLEW_
#define _GLEW_
	#include <GL\glew.h>
	#include <GL\glfw.h>
#endif

#include <iostream>

void ParticleTexture::loadTexture(const char* filepath){
	GLFWimage img;

	if(glfwReadImage(filepath, &img, GLFW_ORIGIN_UL_BIT) == GL_FALSE){
		throw std::exception("ERROR: Could not load texture image!");
	}
	
	glGetError();

	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, img.Format, img.Width, img.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.Data);

	if(glGetError() != GL_NO_ERROR){
		glfwFreeImage(&img);
		glDeleteTextures(1, &_textureID);
		
		throw std::exception("ERROR: Could not initialize particle texture!");
	}

}

void ParticleTexture::useTexture(GLuint shaderProgramID){
	glGetError();

	_uniTexture = glGetUniformLocation(shaderProgramID, "texture");

	if(glGetError() != GL_NO_ERROR){
		throw std::exception("ERROR: Could not get texture-uniform-location!");
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glUniform1i(_uniTexture, 0);
	
}

void ParticleTexture::deleteTexture(){
	glDeleteTextures(1, &_textureID);
}