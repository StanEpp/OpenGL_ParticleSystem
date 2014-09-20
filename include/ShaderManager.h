#ifndef _SHADERMANAGER_
#define _SHADERMANAGER_

#ifndef _GLEW_
#define _GLEW_
	#include <GL\glew.h>
	#include <GLFW\glfw3.h>
#endif

#include <stdexcept>
#include <string>
#include <fstream>

#include "ShaderBaseModel.h"


class ShaderManager{
private:
	ShaderBaseModel*	_shaderData;
	
	static ShaderManager*		_instance;
	
	ShaderManager() : _shaderData(new ShaderBaseModel()){
	}
	
	~ShaderManager(){
		delete _shaderData;
	}
public:

	std::string getFileContents(const char* filename) const; 
	void 				loadShader(const char* filename, const std::string& shaderKey, GLenum type);
	void 				attachShader(const std::string& shaderKey, const std::string& shaderProgramKey);
	void 				detachShader(const std::string& shaderKey, const std::string& shaderProgramKey);
	
	void 				createProgram(const std::string& shaderProgramKey);
	void 				useProgram(const std::string& shaderProgramKey);
	void 				linkProgram(const std::string& shaderProgramKey);
	
	GLuint 			getShaderID(const std::string& shaderKey);
	GLuint 			getShaderProgramID(const std::string& shaderProgramKey);
	
	void 				deleteProgram(const std::string& shaderProgramKey);
	void 				deleteShader(const std::string& shaderKey);
	
	void 				resetProgram();
	
	void 				loadUintUniform(const std::string& shaderProgram, const std::string& name, GLuint value);
	void 				loadFloatUniform(const std::string& shaderProgram, const std::string& name, GLfloat value);
	void 				loadVec4Uniform(const std::string& shaderProgram, const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void 				loadMatrix4Uniform(const std::string& shaderProgram, const std::string& name, const GLfloat* value);
	
	void 				getBufferVariableIndices(const std::string& shaderProgram, int length, const GLchar** names, GLint* indices);
	
	static 			ShaderManager* getInstance();
};


#endif