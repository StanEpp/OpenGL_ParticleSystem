#include "ShaderManager.h"

void ShaderManager::loadShader(const char* filepath, std::string shaderKey, GLenum type){
	try{
		_txtFile->loadFile(filepath);
	} catch(std::exception e){
		throw std::exception((std::string("Could not load file from ")+filepath+": "+e.what()).c_str());
		
	}

	glGetError();

	GLuint shaderID = glCreateShader(type);
	const char* source = _txtFile->c_str();

	if(shaderID != 0){
		_shaderData->saveShader(shaderKey, shaderID);
		glShaderSource(shaderID, 1, &source, NULL);
		glCompileShader(shaderID);
	}

	GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

	if(glGetError() != GL_NO_ERROR || compileStatus == GL_FALSE){

		if(shaderID != 0){
			_shaderData->deleteShader(shaderKey);
		}

		GLsizei* length = new GLsizei;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, length); //Get the length of the compilation log
		char* compilationLog = new char[*length];			 //Create the needed char array

		glGetShaderInfoLog(shaderID, *length, NULL, compilationLog); //Get the compilation log

		throw std::exception(("ERROR: \nCompilation log of shader "+shaderKey+":\n"+std::string(compilationLog)).c_str());
	}
	
	
}

void ShaderManager::attachShader(std::string shaderKey, std::string shaderProgramKey){
	glGetError();

	GLuint shaderID = _shaderData->getShaderID(shaderKey);
	GLuint shaderProgramID = _shaderData->getShaderProgramID(shaderProgramKey);

	if(shaderProgramID != 0 && shaderID != 0){
		glAttachShader(shaderProgramID, shaderID);
	} else {
		throw std::exception("ERROR: No shader with associated key does exist!");
	}

	if(glGetError() != GL_NO_ERROR){
		throw std::exception((std::string("ERROR: Could not attach shader ")+std::string(shaderKey)+std::string(" to ")+std::string(shaderProgramKey)).c_str());
	}
}

void ShaderManager::detachShader(std::string shaderKey, std::string shaderProgramKey){
	glGetError();

	GLuint shaderID = _shaderData->getShaderID(shaderKey);
	GLuint shaderProgramID = _shaderData->getShaderProgramID(shaderProgramKey);

	if(shaderProgramID != 0 && shaderID != 0){
		glDetachShader(shaderProgramID, shaderID);
	} else {
		throw std::exception("ERROR: No shader or shader-program with associated key does exist!");
	}

	if(glGetError() != GL_NO_ERROR){
		throw std::exception(("ERROR: Could not detach shader "+std::string(shaderKey)+" from "+std::string(shaderProgramKey)).c_str());
	}
}

void ShaderManager::resetProgram(){
	glUseProgram(0);
}

void ShaderManager::createProgram(std::string shaderProgramKey){

	glGetError();

	GLuint shaderProgramID = glCreateProgram();

	if(shaderProgramID != 0){
		_shaderData->saveShaderProgram(shaderProgramKey, shaderProgramID);
	}

	if(glGetError() != GL_NO_ERROR){

		if(shaderProgramID != 0){
			_shaderData->deleteShaderProgram(shaderProgramKey);
		}

		throw std::exception(("ERROR: Could not create shader-program "+shaderProgramKey).c_str());
	}
}

void ShaderManager::useProgram(std::string shaderProgramKey){
	glGetError();

	GLuint shaderProgramID = _shaderData->getShaderProgramID(shaderProgramKey);

	if(shaderProgramID != 0){
		glUseProgram(shaderProgramID);
	} else {
		throw std::exception("ERROR: No shader-program with associated key does exist!");
	}


	if(glGetError() != GL_NO_ERROR){
		throw std::exception(("ERROR: Could not use shaderProgram "+shaderProgramKey+"!\n").c_str());
	}
}

void ShaderManager::linkProgram(std::string shaderProgramKey){
	glGetError();

	GLuint shaderProgramID = _shaderData->getShaderProgramID(shaderProgramKey);

	if(shaderProgramID != 0){
		glLinkProgram(shaderProgramID);
	} else {
		throw std::exception("ERROR: No shader-program with associated key does exist!");
	}

	GLint linkStatus;
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &linkStatus);

	if(glGetError() != GL_NO_ERROR || linkStatus == GL_FALSE){

		GLsizei* length = new GLsizei;
		glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, length); //Get the length of the compilation log
		char* linkingLog = new char[*length];			 //Create the needed char array

		glGetProgramInfoLog(shaderProgramID, *length, NULL, linkingLog); //Get the compilation log

		throw std::exception(("ERROR: \nLinker log of shader-programm "+shaderProgramKey+":\n"+std::string(linkingLog)).c_str());

	}
}

GLuint ShaderManager::getShaderID(std::string shaderKey){
	return _shaderData->getShaderID(shaderKey);
}

GLuint ShaderManager::getShaderProgramID(std::string shaderProgramKey){
	return _shaderData->getShaderProgramID(shaderProgramKey);
}

void ShaderManager::deleteProgram(std::string shaderProgramKey){
	glGetError();

	GLuint shaderProgramID = _shaderData->getShaderProgramID(shaderProgramKey);

	if(shaderProgramID != 0){
		glDeleteProgram(shaderProgramID);
	} else {
		throw std::exception("ERROR: No shader-program with associated key does exist!");
	}

	if(glGetError() != GL_NO_ERROR){
		throw std::exception(("ERROR: Could not delete shader-program "+shaderProgramKey).c_str());
	}
}

void ShaderManager::deleteShader(std::string shaderKey){
	glGetError();

	GLuint shaderID = _shaderData->getShaderID(shaderKey);

	if(shaderID != 0){
		glDeleteShader(shaderID);
	} else {
		throw std::exception("ERROR: No shader-program with associated key does exist!");
	}


	if(glGetError() != GL_NO_ERROR){
		throw std::exception(("ERROR: Could not delete shader "+shaderKey).c_str());
	}
}