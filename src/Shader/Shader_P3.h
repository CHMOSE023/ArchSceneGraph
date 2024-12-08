#pragma once
#include "Shader.h"
class Shader_P3 :public Shader
{

public:
    Shader_P3() {};

    ~Shader_P3() {};

    virtual bool  Initialize()
    {
        const char* vs = R"(
               #version 330 core
               layout(location = 0)  in vec3 aPos;            

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
                
                uniform vec4 color;
                
                out vec4 fragment;
                void main()
                {
                   fragment = vec4(color);                   
                }
          )";

        bool    res = CreateShader(vs, ps);

        if (res)
        {
            m_position    = glGetAttribLocation(m_shaderId, "aPos");           
            m_model       = glGetUniformLocation(m_shaderId, "model");
            m_view        = glGetUniformLocation(m_shaderId, "view");
            m_proj        = glGetUniformLocation(m_shaderId, "projection");
            m_color       = glGetUniformLocation(m_shaderId, "color"); // 物体整体颜色

        }

        return true;
    }

public:

    unsigned m_position = -1;
    unsigned m_color = -1;
   
    unsigned m_model = -1;
    unsigned m_view = -1;
    unsigned m_proj = -1;
};
