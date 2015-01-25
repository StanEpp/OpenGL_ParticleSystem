#ifndef _SHADERBASEMODEL_
#define _SHADERBASEMODEL_

#ifndef _GLEW_
#define _GLEW_
	#include <GL\glew.h>
	#include <GFWL\glfw3.h>
#endif
#include <map>
#include <string>
#include <memory>

class ShaderBaseModel{

	friend class ShaderManager;

private:
	std::map<std::string, GLuint>	_shader;
	std::map<std::string, GLuint>	_shaderProgram;

	//May only be instantiated in ShaderManager
	ShaderBaseModel() {}

public:
	~ShaderBaseModel(){
	}
	

	/*Returns ShaderID with the associated Key. Returns 0 if Key does not exist. */
	GLuint getShaderID(const std::string& ShaderKey){
		std::map<std::string, GLuint>::iterator it = _shader.find(ShaderKey);

		if(it == _shader.end()){
			return 0;
		} else {
			return it->second;
		}
		
	}

	/*Returns ShaderID with the associated Key. Returns 0 if Key does not exist. */
	GLuint getShaderProgramID(const std::string& ShaderProgramKey){
		std::map<std::string, GLuint>::iterator it = _shaderProgram.find(ShaderProgramKey);

		if(it == _shaderProgram.end()){
			return 0;
		} else {
			return it->second;
		}
	}

	/*Returns false, if element with the same key already exists, otherwise returns true */
	bool saveShader(const std::string& ShaderKey,const GLuint ShaderID){
		return _shader.insert(std::pair<std::string, GLuint>(ShaderKey, ShaderID) ).second;
	}

	/*Returns false, if element with the same key already exists, otherwise returns true */
	bool saveShaderProgram(const std::string& ShaderProgramKey, const GLuint ShaderProgramID){
		return _shaderProgram.insert(std::pair<std::string, GLuint>(ShaderProgramKey, ShaderProgramID) ).second;
	}

	void deleteShader(const std::string& ShaderKey){
		_shader.erase(ShaderKey);
	}

	void deleteShaderProgram(const std::string& ShaderProgramKey){
		_shaderProgram.erase(ShaderProgramKey);
	}

};


#endif