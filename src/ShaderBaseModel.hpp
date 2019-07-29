#pragma once

#include <unordered_map>
#include <string>

#include <GL/gl3w.h>

class ShaderBaseModel
{
    friend class ShaderManager;

private:
    //May only be instantiated in ShaderManager
    ShaderBaseModel() {}

public:
    /*Returns ShaderID with the associated Key. Returns 0 if Key does not exist. */
    GLuint getShaderID(const std::string& shaderKey)
    {
        auto it = m_shader.find(shaderKey);

        if (it == m_shader.end()) {
            return 0;
        } else {
            return it->second;
        }
    }

    /*Returns ShaderID with the associated Key. Returns 0 if Key does not exist. */
    GLuint getShaderProgramID(const std::string& shaderProgramKey)
    {
        auto it = m_shaderProgram.find(shaderProgramKey);

        if (it == m_shaderProgram.end()) {
            return 0;
        } else {
            return it->second;
        }
    }

    /*Returns false, if element with the same key already exists, otherwise returns true */
    bool saveShader(const std::string& shaderKey,const GLuint ShaderID)
    {
        return m_shader.insert(std::make_pair(shaderKey, ShaderID)).second;
    }

    /*Returns false, if element with the same key already exists, otherwise returns true */
    bool saveShaderProgram(const std::string& shaderProgramKey, const GLuint shaderProgramID)
    {
        return m_shaderProgram.insert(std::make_pair(shaderProgramKey, shaderProgramID)).second;
    }

    void deleteShader(const std::string& shaderKey)
    {
        m_shader.erase(shaderKey);
    }

    void deleteShaderProgram(const std::string& shaderProgramKey)
    {
        m_shaderProgram.erase(shaderProgramKey);
    }

private:
    std::unordered_map<std::string, GLuint> m_shader;
    std::unordered_map<std::string, GLuint> m_shaderProgram;
};