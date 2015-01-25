#include "shaderManager.h"

std::string	ShaderManager::getFileContents(const std::string& filename) const{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in){
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw std::runtime_error("Could not open file: "+ filename+ "!");
}

void ShaderManager::loadShader(const std::string& filename, const std::string& shaderKey, const GLenum type){
	std::string shaderCode;
	try{
		shaderCode = getFileContents(filename);
	} catch(std::exception& e){
		throw std::runtime_error("Could not load file \""+filename+"\" due to error: "+e.what());
	}

	glGetError();

	GLuint shaderID = glCreateShader(type);
	const char* source = shaderCode.c_str();

	if(shaderID != 0){
		_shaderData.saveShader(shaderKey, shaderID);
		glShaderSource(shaderID, 1, &source, NULL);
		glCompileShader(shaderID);
	}

	GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

	if(glGetError() != GL_NO_ERROR || compileStatus == GL_FALSE){

		if(shaderID != 0){
			_shaderData. deleteShader(shaderKey);
		}

		GLsizei length;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length); //Get the length of the compilation log
		char* compilationLog = new char[length];			 //Create the needed char array to store the log
		glGetShaderInfoLog(shaderID, length, NULL, compilationLog); //Get the compilation log
		std::string compilationLogString(compilationLog); //Create string for the compilation log
		delete[] compilationLog; //Delete the compilation log array
		
		throw std::runtime_error(("ERROR: \nCompilation log of shader "+shaderKey+":\n"+compilationLogString).c_str());
	}
	
}

void ShaderManager::attachShader(const std::string& shaderKey,  const std::string& shaderProgramKey){
	glGetError();

	GLuint shaderID = _shaderData.getShaderID(shaderKey);
	GLuint shaderProgramID = _shaderData.getShaderProgramID(shaderProgramKey);

	if(shaderProgramID != 0 && shaderID != 0){
		glAttachShader(shaderProgramID, shaderID);
	} else {
		throw std::runtime_error("ERROR: No shader with associated key does exist!");
	}

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not attach shader "+shaderKey+" to "+shaderProgramKey).c_str());
	}
}

void ShaderManager::detachShader(const std::string& shaderKey, const std::string& shaderProgramKey){
	glGetError();

	GLuint shaderID = _shaderData.getShaderID(shaderKey);
	GLuint shaderProgramID = _shaderData.getShaderProgramID(shaderProgramKey);

	if(shaderProgramID != 0 && shaderID != 0){
		glDetachShader(shaderProgramID, shaderID);
	} else {
		throw std::runtime_error("ERROR: No shader or shader-program with associated key does exist!");
	}

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not detach shader "+shaderKey+" from "+shaderProgramKey).c_str());
	}
}

void ShaderManager::resetProgram(){
	glUseProgram(0);
}

void ShaderManager::createProgram(const std::string& shaderProgramKey){

	glGetError();

	GLuint shaderProgramID = glCreateProgram();

	if(shaderProgramID != 0){
		_shaderData.saveShaderProgram(shaderProgramKey, shaderProgramID);
	}

	if(glGetError() != GL_NO_ERROR){

		if(shaderProgramID != 0){
			_shaderData.deleteShaderProgram(shaderProgramKey);
		}

		throw std::runtime_error(("ERROR: Could not create shader-program "+shaderProgramKey).c_str());
	}
}

void ShaderManager::useProgram(const std::string& shaderProgramKey){
	glGetError();

	GLuint shaderProgramID = _shaderData.getShaderProgramID(shaderProgramKey);

	if(shaderProgramID != 0){
		glUseProgram(shaderProgramID);
	} else {
		throw std::runtime_error("ERROR: No shader-program with associated key does exist!");
	}

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not use shaderProgram "+shaderProgramKey+"!\n").c_str());
	}
}

void ShaderManager::linkProgram(const std::string& shaderProgramKey){
	glGetError();

	GLuint shaderProgramID = _shaderData.getShaderProgramID(shaderProgramKey);

	if(shaderProgramID != 0){
		glLinkProgram(shaderProgramID);
	} else {
		throw std::runtime_error("ERROR: No shader-program with associated key does exist!");
	}

	GLint linkStatus;
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &linkStatus);

	if(glGetError() != GL_NO_ERROR || linkStatus == GL_FALSE){

		GLsizei length;
		glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &length); //Get the length of the compilation log
		char* linkingLog = new char[length];			 //Create the needed char array to store the log
		glGetProgramInfoLog(shaderProgramID, length, NULL, linkingLog); //Get the compilation log
		std::string linkingLogString(linkingLog);	//Save the linking log in a string
		delete[] linkingLog;	//Free the allocated memory

		throw std::runtime_error(("ERROR: \nLinker log of shader-programm "+shaderProgramKey+":\n"+linkingLogString).c_str());

	}
}

GLuint ShaderManager::getShaderID(const std::string& shaderKey){
	return _shaderData.getShaderID(shaderKey);
}

GLuint ShaderManager::getShaderProgramID(const std::string& shaderProgramKey){
	return _shaderData.getShaderProgramID(shaderProgramKey);
}

void ShaderManager::deleteProgram(const std::string& shaderProgramKey){
	glGetError();

	GLuint shaderProgramID = _shaderData.getShaderProgramID(shaderProgramKey);

	if(shaderProgramID != 0){
		glDeleteProgram(shaderProgramID);
	} else {
		throw std::runtime_error("ERROR: No shader-program with associated key does exist!");
	}

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not delete shader-program "+shaderProgramKey).c_str());
	}
}

void ShaderManager::deleteShader(const std::string& shaderKey){
	glGetError();

	GLuint shaderID = _shaderData.getShaderID(shaderKey);

	if(shaderID != 0){
		glDeleteShader(shaderID);
	} else {
		throw std::runtime_error("ERROR: No shader-program with associated key does exist!");
	}

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not delete shader "+shaderKey).c_str());
	}
}

ShaderManager* ShaderManager::_instance = NULL;

ShaderManager* ShaderManager::getInstance(){
	if(_instance == NULL){
		_instance = new ShaderManager();
	}

	return _instance;
}

void ShaderManager::loadUintUniform(const std::string& shaderProgram, const std::string& name, const GLuint value){
	glGetError();

	int _uniID = glGetUniformLocation(getShaderProgramID(shaderProgram), name.c_str());

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not get "+name+"-uniform-location!").c_str());
	}

	glUniform1ui(_uniID, value);

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not update "+name+"-uniform-location!").c_str());
	}
}

void ShaderManager::loadFloatUniform(const std::string& shaderProgram, const std::string& name, const GLfloat value){
	glGetError();

	int _uniID = glGetUniformLocation(getShaderProgramID(shaderProgram), name.c_str());

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not get "+name+"-uniform-location!").c_str());
	}

	glUniform1f(_uniID, value);

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not update "+name+"-uniform-location!").c_str());
	}
}

void ShaderManager::loadVec4Uniform(const std::string& shaderProgram, const std::string& name, const GLfloat x, const GLfloat y, const GLfloat z, const GLfloat w){
	glGetError();
	int _uniID = glGetUniformLocation(getShaderProgramID(shaderProgram), name.c_str());

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not get "+name+"-uniform-location!").c_str());
	}

	glUniform4f(_uniID, x, y, z, w);

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not update "+name+"-uniform-location!").c_str());
	}
}

void ShaderManager::loadMatrix4Uniform(const std::string& shaderProgram, const std::string& name, const GLfloat* value){
	glGetError();
	int _uniID = glGetUniformLocation(getShaderProgramID(shaderProgram), name.c_str());

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not get "+name+"-uniform-location!").c_str());
	}

	glUniformMatrix4fv(_uniID, 1, GL_FALSE, value);

	if(glGetError() != GL_NO_ERROR){
		throw std::runtime_error(("ERROR: Could not update "+name+"-uniform-location!").c_str());
	}
}

void ShaderManager::getBufferVariableIndices(const std::string& shaderProgram, const int length, const GLchar** names, GLint* indices){
	for(int i = 0; i < length; ++i){
		indices[i] = glGetProgramResourceIndex(getShaderProgramID(shaderProgram), GL_BUFFER_VARIABLE, names[i]);
	}
}