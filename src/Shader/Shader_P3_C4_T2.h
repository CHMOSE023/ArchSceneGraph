#pragma once
#include "Shader.h"
class Shader_P3_C4_T2 :public Shader
{	
public:
    Shader_P3_C4_T2()
	{
		m_mvp      = -1;
		m_position = -1;
		m_color    = -1;
		m_uv       = -1;
		m_texture  = -1;
	};

	~Shader_P3_C4_T2()
	{
	};

	virtual bool  Initialize()
	{
       const char* vs = R"(
               #version 330
               uniform   mat4   MVP;
               layout(location = 0)  in vec3   aPos;
               layout(location = 1)  in vec4   aColor;
               layout(location = 2)  in vec2   aTexCoord;

               out       vec4   outColor;
               out       vec2   outTexCoord; 
      
               void main()
               {
                  gl_Position = MVP * vec4(aPos,  1.0);
                  outColor    = aColor;
                  outTexCoord = aTexCoord;
               }
          )";
       
        const char* ps = R"(
                #version 330
                uniform   sampler2D texture;
                in        vec4      outColor;
                in        vec2      outTexCoord;

                out       vec4      fragment;       
                void main()
                {
                    vec4 textureColor = texture2D(texture,outTexCoord);
                    //fragment          = textureColor * outColor;
                    if(textureColor.a<=0.2) // »ìºÏ
                    {
                       discard;
                    }else{
                      fragment = textureColor * outColor;
                    }                  
                }
          )";
       
        bool    res = CreateShader(vs, ps);
       
        if (res)
        {
            m_mvp       = glGetUniformLocation(m_shaderId, "MVP");
            m_texture   = glGetUniformLocation(m_shaderId, "texture");
            m_position  = glGetAttribLocation(m_shaderId, "aPos");
            m_color     = glGetAttribLocation(m_shaderId, "aColor");
            m_uv        = glGetAttribLocation(m_shaderId, "aTexCoord");

         
        }	

        return res;
	}

public:   
    int m_mvp;
    int m_position;
    int m_color;
    int m_uv;
    int m_texture;
};

