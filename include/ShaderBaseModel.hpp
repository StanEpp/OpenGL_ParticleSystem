#ifndef _SHADERBASEMODEL_
#define _SHADERBASEMODEL_

#ifndef _GLEW_
#define _GLEW_
  #include <GL\gl3w.h>
  #include <GLFW\glfw3.h>
#endif

#include <map>
#include <string>

class ShaderBaseModel{

  friend class ShaderManager;

private:
  std::map<std::string, GLuint> _shader;
  std::map<std::string, GLuint> _shaderProgram;

  //May only be instantiated in ShaderManager
  ShaderBaseModel() {}
  ShaderBaseModel(ShaderBaseModel&) = delete;
  ShaderBaseModel(ShaderBaseModel&&) = delete;
  ShaderBaseModel& operator=(ShaderBaseModel&) = delete;
  ShaderBaseModel& operator=(ShaderBaseModel&&) = delete;

public:

  // Returns shader ID with the associated Key. Returns 0 if Key does not exist.
  GLuint getShaderID(const std::string& ShaderKey){
    auto it = _shader.find(ShaderKey);
    if(it == _shader.end()) return 0;
    return it->second;
  }

  // Returns shader ID with the associated Key. Returns 0 if Key does not exist.
  GLuint getShaderProgramID(const std::string& ShaderProgramKey){
    auto it = _shaderProgram.find(ShaderProgramKey);
    if(it == _shaderProgram.end()) return 0;
    return it->second;
  }

  // Returns false if element with the same key already exists, otherwise returns true.
  bool saveShader(const std::string& ShaderKey, GLuint ShaderID){
    return _shader.insert(std::make_pair(ShaderKey, ShaderID)).second;
  }

  // Returns false if element with the same key already exists, otherwise returns true.
  bool saveShaderProgram(const std::string& ShaderProgramKey, GLuint ShaderProgramID){
    return _shaderProgram.insert(std::make_pair(ShaderProgramKey, ShaderProgramID)).second;
  }

  void deleteShader(const std::string& ShaderKey){
    _shader.erase(ShaderKey);
  }

  void deleteShaderProgram(const std::string& ShaderProgramKey){
    _shaderProgram.erase(ShaderProgramKey);
  }

};


#endif