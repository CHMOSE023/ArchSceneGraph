#pragma once 
#include  "../Shader/Shader.h"
#include  <map>
#include <string>
// ¹âÔ´ÐÎ×´
class ShaderCube : public Shader
{
public:
    ShaderCube() {};
	~ShaderCube() {};

    virtual bool  Initialize()
    {
        const char* vs = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;
            
            void main()
            {
            	gl_Position = projection * view * model * vec4(aPos, 1.0);
            }
          )";

        const char* ps = R"(
            #version 330 core

            out vec4 FragColor;
            
            void main()
            {
                FragColor = vec4(1.0); // set all 4 vector values to 1.0
            }
          )";

        bool    res = CreateShader(vs, ps);

        if (res)
        {
            m_position = glGetAttribLocation(m_shaderId, "aPos"); 

            m_model    = glGetUniformLocation(m_shaderId, "model");
            m_view     = glGetUniformLocation(m_shaderId, "view");
            m_proj     = glGetUniformLocation(m_shaderId, "projection");

            uniform["model"]     = m_model;
            uniform["view"]      = m_view;
            uniform["projection"]= m_proj;
        }
        return true;
    }

public:
    unsigned m_position = -1;
    unsigned m_model    = -1;
    unsigned m_view     = -1;
    unsigned m_proj     = -1;

};

