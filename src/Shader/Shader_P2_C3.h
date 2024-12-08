#pragma once
#include "Shader.h"
class Shader_P2_C3:public Shader
{

public:
    Shader_P2_C3()
	{
		m_mvp      = -1;
		m_position = -1;
		m_color    = -1;
	};

	~Shader_P2_C3()
	{
	};

	virtual bool  Initialize()
	{
       const char* vs = R"(
               #version 330
               uniform mat4 MVP;
               in vec2 vPos;
               in vec3 vCol;

               out vec3 color;
               void main()
               {
                  gl_Position = MVP * vec4(vPos, 0.0, 1.0);
                  color = vCol;
               }
          )";

        const char* ps = R"(
                #version 330
                in vec3 color;
                out vec4 fragment;
                void main()
                {
                     fragment = vec4(color, 1.0);
                    //fragment = vec4(1.0,1,0,0);
                }
          )";

        bool    res = CreateShader(vs, ps);

        if (res)
        {
            m_mvp      = glGetUniformLocation(m_shaderId, "MVP");
            m_position = glGetAttribLocation(m_shaderId,  "vPos");
            m_color    = glGetAttribLocation(m_shaderId,  "vCol");;
        }	

        return true;
	}

public:
	int m_mvp;
    int m_position;
	int m_color;
};

