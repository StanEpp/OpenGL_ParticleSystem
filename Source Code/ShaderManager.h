#ifndef _SHADERMANAGER_
#define _SHADERMANAGER_

#ifndef _GLEW_
#define _GLEW_
	#include <GL\glew.h>
	#include <GL\glfw.h>
#endif
#include "ShaderBaseModel.h"
#include <boost\shared_ptr.hpp>
#include "textFile.h"

class ShaderManager{
private:
	boost::shared_ptr<textFile>			_txtFile;
	ShaderBaseModel*					_shaderData;

public:
	ShaderManager() : _txtFile(new textFile(false)), _shaderData(new ShaderBaseModel()) {}
	~ShaderManager(){
		delete _shaderData;
	}
	
	void loadShader(const char* filepath, std::string shaderKey, GLenum type);
	void attachShader(std::string shaderKey, std::string shaderProgramKey);
	void detachShader(std::string shaderKey, std::string shaderProgramKey);

	void createProgram(std::string shaderProgramKey);
	void useProgram(std::string shaderProgramKey);
	void linkProgram(std::string shaderProgramKey);

	GLuint getShaderID(std::string shaderKey);
	GLuint getShaderProgramID(std::string shaderProgramKey);
	

	void deleteProgram(std::string shaderProgramKey);
	void deleteShader(std::string shaderKey);

	void resetProgram();


};


#endif