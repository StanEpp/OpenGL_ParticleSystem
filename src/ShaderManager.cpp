#include "ShaderManager.hpp"

/////// Specialization of the uploadUniforms template method
template<>
void ShaderManager::uploadUniforms<GLfloat, GLfloat, GLfloat, GLfloat>(GLuint uniID, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
  glUniform4f(uniID, x, y, z, w);
}

template<>
void ShaderManager::uploadUniforms<GLfloat, GLfloat, GLfloat>(GLuint uniID, GLfloat x, GLfloat y, GLfloat z){
  glUniform3f(uniID, x, y, z);
}

template<>
void ShaderManager::uploadUniforms<GLfloat, GLfloat>(GLuint uniID, GLfloat x, GLfloat y){
  glUniform2f(uniID, x, y);
}

template<>
void ShaderManager::uploadUniforms<GLfloat>(GLuint uniID, GLfloat x){
  glUniform1f(uniID, x);
}

template<>
void ShaderManager::uploadUniforms<GLint>(GLuint uniID, GLint x){
  glUniform1i(uniID, x);
}

template<>
void ShaderManager::uploadUniforms<GLuint>(GLuint uniID, GLuint x){
  glUniform1ui(uniID, x);
}
//////////////////////////////////////////////////////////////////////

ShaderManager::ShaderManager(){
  std::fill(_ubo_points.begin(), _ubo_points.end(), false);
}

std::string ShaderManager::getFileContents(const std::string& filename) const{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in){
    std::string contents;
    in.seekg(0, std::ios::end);
    auto length = in.tellg();
    if (length != -1){
      contents.resize(static_cast<unsigned int>(length));
      in.seekg(0, std::ios::beg);
      in.read(&contents[0], contents.size());
      in.close();
      return contents;
    }
    throw std::runtime_error("Failed reading buffer position of file: " + filename + " !");
  }
  throw std::runtime_error("Could not open file: " + filename + " !");
}

void ShaderManager::loadShader(const std::string& filename, const std::string& shaderKey, GLenum type){
  auto shaderCode = getFileContents(filename);

  glGetError();

  GLuint shaderID = glCreateShader(type);
  const char* source = shaderCode.c_str();

  if (shaderID != 0){
    _shaderData.saveShader(shaderKey, shaderID);
    glShaderSource(shaderID, 1, &source, NULL);
    glCompileShader(shaderID);
  }

  GLint compileStatus;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

  if (glGetError() != GL_NO_ERROR || compileStatus == GL_FALSE){

    if (shaderID != 0) _shaderData.deleteShader(shaderKey);

    GLsizei length;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);       //Get the length of the compilation log
    auto compilationLog = new char[length];                    //Create the needed char array to store the log
    glGetShaderInfoLog(shaderID, length, NULL, compilationLog); //Get the compilation log
    std::string compilationLogString(compilationLog);           //Create string for the compilation log
    delete[] compilationLog;                                    //Delete the compilation log array

    throw std::runtime_error("ERROR: \nCompilation log of shader " + shaderKey + ": \n" + compilationLogString);
  }
}

void ShaderManager::attachShader(const std::string& shaderKey, const std::string& shaderProgramKey){
  glGetError();

  GLuint shaderID = _shaderData.getShaderID(shaderKey);
  GLuint shaderProgramID = _shaderData.getShaderProgramID(shaderProgramKey);

  if (shaderProgramID != 0 && shaderID != 0){
    glAttachShader(shaderProgramID, shaderID);
  }
  else {
    throw std::runtime_error("ERROR: No shader with associated key does exist!");
  }

  if (glGetError() != GL_NO_ERROR){
    throw std::runtime_error("ERROR: Could not attach shader " + shaderKey + " to " + shaderProgramKey);
  }
}

void ShaderManager::detachShader(const std::string& shaderKey, const std::string& shaderProgramKey){
  glGetError();

  GLuint shaderID = _shaderData.getShaderID(shaderKey);
  GLuint shaderProgramID = _shaderData.getShaderProgramID(shaderProgramKey);

  if (shaderProgramID != 0 && shaderID != 0){
    glDetachShader(shaderProgramID, shaderID);
  }
  else {
    throw std::runtime_error("ERROR: No shader or shader-program with associated key does exist!");
  }

  if (glGetError() != GL_NO_ERROR){
    throw std::runtime_error("ERROR: Could not detach shader " + shaderKey + " from " + shaderProgramKey);
  }
}

void ShaderManager::resetProgram(){
  glUseProgram(0);
}


GLuint ShaderManager::createProgram(const std::string& shaderProgramKey){
  glGetError();

  GLuint shaderProgramID = glCreateProgram();

  if (shaderProgramID != 0) _shaderData.saveShaderProgram(shaderProgramKey, shaderProgramID);

  if (glGetError() != GL_NO_ERROR){
    
    if (shaderProgramID != 0) _shaderData.deleteShaderProgram(shaderProgramKey);
    
    throw std::runtime_error("ERROR: Could not create shader-program " + shaderProgramKey);
  }
  
  return shaderProgramID;
}

GLuint ShaderManager::createProgram(){
  glGetError();

  GLuint shaderProgramID = glCreateProgram();

  if (glGetError() != GL_NO_ERROR) throw std::runtime_error("ERROR: Could not create shader-program");

  return shaderProgramID;
}


void ShaderManager::useProgram(const std::string& shaderProgramKey){
  GLuint shaderProgramID = _shaderData.getShaderProgramID(shaderProgramKey);
  useProgram(shaderProgramID);
}

void ShaderManager::useProgram(GLuint shaderProgramID){
  glGetError();

  if (shaderProgramID != 0){
    glUseProgram(shaderProgramID);
  }
  else {
    throw std::runtime_error("ERROR: No shader-program with ID less or equal than 0 exists!");
  }

  if (glGetError() != GL_NO_ERROR) throw std::runtime_error("ERROR: Could not use shaderProgram with ID " + std::to_string(shaderProgramID) + "!\n");
}


void ShaderManager::linkProgram(const std::string& shaderProgramKey){
  GLuint shaderProgramID = _shaderData.getShaderProgramID(shaderProgramKey);
  linkProgram(shaderProgramID);
}

void ShaderManager::linkProgram(GLuint shaderProgramID){
  glGetError();

  if (shaderProgramID != 0){
    glLinkProgram(shaderProgramID);
  }
  else {
    throw std::runtime_error("ERROR: Can't link shader program with invalid ID " + std::to_string(shaderProgramID) + "!");
  }

  GLint linkStatus;
  glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &linkStatus);

  if (glGetError() != GL_NO_ERROR || linkStatus == GL_FALSE){
    GLsizei length;
    glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &length);   //Get the length of the compilation log
    auto linkingLog = new char[length];                            //Create the needed char array to store the log
    glGetProgramInfoLog(shaderProgramID, length, NULL, linkingLog); //Get the compilation log
    std::string linkingLogString(linkingLog);                       //Save the linking log in a string
    delete[] linkingLog;                                            //Free the allocated memory

    throw std::runtime_error("ERROR: \nLinker log of shader-programm:\n" + linkingLogString);
  }
}


GLuint ShaderManager::getShaderID(const std::string& shaderKey){
  return _shaderData.getShaderID(shaderKey);
}

GLuint ShaderManager::getShaderProgramID(const std::string& shaderProgramKey){
  return _shaderData.getShaderProgramID(shaderProgramKey);
}

GLuint ShaderManager::occupyUBOBindingPoint(){
  for (unsigned int i = 0; i < GL_MAX_UNIFORM_BUFFER_BINDINGS; i++){
    if (_ubo_points[i] == false){
      _ubo_points[i] = true;
      return i;
    }
  }
  return -1;
}

void ShaderManager::unoccupyUBOBindingPoint(GLuint slot){
  if (slot >= 0 || slot < GL_MAX_UNIFORM_BUFFER_BINDINGS){
    _ubo_points[slot] = false;
  }
}


void ShaderManager::deleteProgram(const std::string& shaderProgramKey){
  auto shaderProgramID = _shaderData.getShaderProgramID(shaderProgramKey);
  if (shaderProgramID == 0) return;
  try{
    deleteProgram(shaderProgramID);
  }
  catch (std::exception& e){
    throw std::runtime_error("Delete shader-program: \"" + shaderProgramKey + "\"\t " + e.what());
    return;
  }
  _shaderData.deleteShaderProgram(shaderProgramKey);
}

void ShaderManager::deleteProgram(GLuint shaderProgramID){
  glGetError();
  
  if (shaderProgramID != 0) glDeleteProgram(shaderProgramID);

  auto err = glGetError();
  if (err != GL_NO_ERROR){
    throw std::runtime_error("ID: " + std::to_string(shaderProgramID) + "  ErrCode: " + errVal(err));
  }
}


void ShaderManager::deleteShader(const std::string& shaderKey){
  auto shaderID = _shaderData.getShaderID(shaderKey);
  deleteShader(shaderID);
  _shaderData.deleteShader(shaderKey);
}

void ShaderManager::deleteShader(GLuint shaderID){
  glGetError();

  if (shaderID != 0) glDeleteShader(shaderID);

  if (glGetError() != GL_NO_ERROR){
    throw std::runtime_error("ERROR: Could not delete shader with ID \"" + std::to_string(shaderID));
  }
}

void ShaderManager::loadMatrix4(const std::string& shaderProgram, const std::string& name, const GLfloat* value){
  loadMatrix4(getShaderProgramID(shaderProgram), name, value);
}

void ShaderManager::loadMatrix4(GLuint shaderProgramID, const std::string& name, const GLfloat* value){
  glGetError();
  
  auto _uniID = glGetUniformLocation(shaderProgramID, name.c_str());

  if (glGetError() != GL_NO_ERROR){
    throw std::runtime_error("ERROR: Could not get " + name + "-uniform-location!");
  }

  loadMatrix4(_uniID, value);
}

void ShaderManager::loadMatrix4(GLint location, const GLfloat* value){
  if (location == -1){
    throw std::runtime_error("ERROR: -1 is not a valid uniform location!");
    return;
  }
  
  glGetError();
  
  glUniformMatrix4fv(location, 1, GL_FALSE, value);

  if (glGetError() != GL_NO_ERROR){
    throw std::runtime_error("ERROR: Could not update uniform with location " + std::to_string(location) + " !");
  }
}

void ShaderManager::getBufferVariableIndices(const std::string& shaderProgram, int length, const GLchar** names, GLint* indices){
  for (int i = 0; i < length; ++i){
    indices[i] = glGetProgramResourceIndex(getShaderProgramID(shaderProgram), GL_BUFFER_VARIABLE, names[i]);
  }
}

GLint ShaderManager::getUniformLocation(GLuint shaderProgramID, const std::string& name){
  return glGetUniformLocation(shaderProgramID, name.c_str());
}

GLint ShaderManager::getUniformLocation(const std::string& shaderProgram, const std::string& name){
  return getUniformLocation(getShaderProgramID(shaderProgram), name);
}

const std::string ShaderManager::errVal(GLenum error){
  if (error == GL_NO_ERROR){
    return "GL_NO_ERROR";
  }
  else if (error == GL_INVALID_ENUM){
    return "GL_INVALID_ENUM";
  }
  else if (error == GL_INVALID_VALUE){
    return "GL_INVALID_VALUE";
  }
  else if (error == GL_INVALID_OPERATION) {
    return "GL_INVALID_OPERATION";
  }
  else if (error == GL_INVALID_FRAMEBUFFER_OPERATION) {
    return "GL_INVALID_FRAMEBUFFER_OPERATION";
  }
  else if (error == GL_OUT_OF_MEMORY) {
    return "GL_OUT_OF_MEMORY";
  }
  else if (error == GL_STACK_OVERFLOW) {
    return "GL_STACK_OVERFLOW";
  }
  else if (error == GL_STACK_UNDERFLOW) {
    return "GL_STACK_UNDERFLOW";
  }

  return "UNKNOWN";
}

// ShaderManager* ShaderManager::_instance = nullptr;

// ShaderManager* ShaderManager::Inst(){
  // if (_instance == nullptr){
    // _instance = new ShaderManager();
  // }

  // return _instance;
// }
