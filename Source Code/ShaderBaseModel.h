#ifndef _SHADERBASEMODEL_
#define _SHADERBASEMODEL_

#ifndef _GLEW_
#define _GLEW_
	#include <GL\glew.h>
	#include <GL\glfw.h>
#endif
#include <map>
#include <string>
#include <boost\shared_ptr.hpp>

class ShaderBaseModel{

	friend class ShaderManager;

private:
	boost::shared_ptr<std::map<std::string, GLuint> >	_shader;
	boost::shared_ptr<std::map<std::string, GLuint> >	_shaderProgram;

	//May only be instantiated in ShaderManager
	ShaderBaseModel() : _shader(new std::map<std::string, GLuint>()),
						_shaderProgram(new std::map<std::string, GLuint>()) {}

	~ShaderBaseModel(){
		_shader->clear();
		_shaderProgram->clear();
	}

public:

	/*Returns ShaderID with the associated Key. Returns 0 if Key does not exist. */
	GLuint getShaderID(std::string ShaderKey){
		std::map<std::string, GLuint>::iterator it = _shader->find(ShaderKey);

		if(it == _shader->end()){
			return 0;
		} else {
			return it->second;
		}
		
	}

	/*Returns ShaderID with the associated Key. Returns 0 if Key does not exist. */
	GLuint getShaderProgramID(std::string ShaderProgramKey){
		std::map<std::string, GLuint>::iterator it = _shaderProgram->find(ShaderProgramKey);

		if(it == _shaderProgram->end()){
			return 0;
		} else {
			return it->second;
		}
	}

	/*Returns false, if element with the same key already exists, otherwise returns true */
	bool saveShader(std::string ShaderKey, GLuint ShaderID){
		return _shader->insert(std::pair<std::string, GLuint>(ShaderKey, ShaderID) ).second;	
	}

	/*Returns false, if element with the same key already exists, otherwise returns true */
	bool saveShaderProgram(std::string ShaderProgramKey, GLuint ShaderProgramID){
		return _shaderProgram->insert(std::pair<std::string, GLuint>(ShaderProgramKey, ShaderProgramID) ).second;
	}

	void deleteShader(std::string ShaderKey){
		_shader->erase(ShaderKey);
	}

	void deleteShaderProgram(std::string ShaderProgramKey){
		_shaderProgram->erase(ShaderProgramKey);
	}

};


#endif