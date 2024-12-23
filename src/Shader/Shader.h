#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <map>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>

class Shader
{
public:
	Shader();
	~Shader();

public:
	bool CreateShader(const char* vertex, const char* fragment);
	virtual void Begin();
	virtual void End();

    void SetVec3(const std::string& name, const float x, const float y, const float z)const
    {
        auto cit = uniform.find(name);
        if (cit == uniform.end())
        {
            //std::cerr << "SetVec3 -> Uniform '" << name << "' not found!" << std::endl;
            return; // 或者可以抛出异常
        }
        glUniform3f(cit->second, x, y, z);
    }

    void SetVec3(const std::string& name, const glm::vec3& vec) const
    {
        auto cit = uniform.find(name);
        if (cit == uniform.end())
        {
            // std::cerr << "SetVec3 -> Uniform '" << name << "' not found!" << std::endl;
            return; // 或者可以抛出异常
        }
        glUniform3f(cit->second, vec.x, vec.y, vec.z);
    }

    void SetMat4(const std::string& name, const glm::mat4& mat) const
    {
        auto cit = uniform.find(name);
        if (cit == uniform.end())
        {
            //std::cerr << "SetMat4 -> Uniform '" << name << "' not found!" << std::endl;
            return; // 或者可以抛出异常
        }
        glUniformMatrix4fv(cit->second, 1, GL_FALSE, glm::value_ptr(mat));         
    }

    void SetFloat(const std::string& name, const float x) const
    {
        auto cit = uniform.find(name);
        if (cit == uniform.end())
        {
            // std::cerr << "SetVec3 -> Uniform '" << name << "' not found!" << std::endl;
            return; // 或者可以抛出异常
        }
        glUniform1f(cit->second, x);
    }

    void SetInt(const std::string& name, const int d)const
    {
        auto cit = uniform.find(name);
        if (cit == uniform.end())
        {
            //  std::cerr << "SetVec3 -> Uniform '" << name << "' not found!" << std::endl;
            return; // 或者可以抛出异常
        }
        glUniform1i(cit->second, d);        
    }
public:
	int  m_shaderId;

    std::map <std::string, unsigned> uniform;
};

