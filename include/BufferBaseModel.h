#ifndef _BUFFERBASEMODEL_
#define _BUFFERBASEMODEL_

#ifndef _GLEW_
#define _GLEW_
	#include <GL\glew.h>
	#include <GLFW\glfw3.h>
#endif

#include <memory>
#include <string>
#include <map>

#include "Classes.h"

class BufferBaseModel{
	friend class ParticleManager;

private:
	std::map<std::string, GLuint>	_buffer;

public:

	/*Returns bufferID with the associated Key. Returns 0 if Key does not exist. */
	GLuint getBufferID(std::string bufferKey) const{
		auto it = _buffer.find(bufferKey);

		if(it == _buffer.end()){
			return 0;
		} else {
			return it->second;
		}
		
	}

	/*Returns false, if element with the same key already exists, otherwise returns true */
	bool saveBuffer(const std::string& bufferKey, GLuint bufferID){
		return _buffer.insert(std::pair<std::string, GLuint>(bufferKey, bufferID) ).second;
	}

	void deleteBuffer(const std::string& ShaderKey){
		_buffer.erase(ShaderKey);
	}

	GLuint removeHead(){
		if(!_buffer.empty()){
			GLuint ID = _buffer.begin()->second;
			_buffer.erase(_buffer.begin());
			return ID;
		} else {
			return 0;
		}
	}

};

#endif