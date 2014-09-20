#ifndef _BUFFERBASEMODEL_
#define _BUFFERBASEMODEL_

#ifndef _GLEW_
#define _GLEW_
	#include <GL\glew.h>
	#include <GLFW\glfw3.h>
#endif

#include <boost\shared_ptr.hpp>

#include <string>
#include <map>

#include "Classes.h"

class BufferBaseModel{
	friend class ParticleManager;
private:
	boost::shared_ptr<std::map<std::string, GLuint> >	_buffer;

	BufferBaseModel() : _buffer(new std::map<std::string, GLuint>()){}

public:

	/*Returns bufferID with the associated Key. Returns 0 if Key does not exist. */
	GLuint getBufferID(std::string bufferKey){
		std::map<std::string, GLuint>::iterator it = _buffer->find(bufferKey);

		if(it == _buffer->end()){
			return 0;
		} else {
			return it->second;
		}
		
	}

	/*Returns false, if element with the same key already exists, otherwise returns true */
	bool saveBuffer(std::string bufferKey, GLuint bufferID){
		return _buffer->insert(std::pair<std::string, GLuint>(bufferKey, bufferID) ).second;	
	}

	void deleteBuffer(std::string ShaderKey){
		_buffer->erase(ShaderKey);
	}

	GLuint removeHead(){
		if(!_buffer->empty()){
			GLuint ID = _buffer->begin()->second;
			_buffer->erase(_buffer->begin());
			return ID;
		} else {
			return 0;
		}
	}

};

#endif