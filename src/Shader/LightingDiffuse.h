#pragma once
#include "Shader.h"
class LightingDiffuse :public Shader
{

public:
    LightingDiffuse()
    {       
    };

    ~LightingDiffuse()
    {
    };

    virtual bool  Initialize()
    {
        const char* vs = R"(
               #version 330 core
               layout(location = 0)  in vec3 aPos;
               layout(location = 1)  in vec3 aNormal;

               uniform mat4 model;
               uniform mat4 view;
               uniform mat4 projection;

               out vec3 FragPos; // ?
               out vec3 Normal;

               void main()
               {
                  FragPos= vec3(model * vec4(aPos, 1.0));
                  Normal = aNormal;

                  gl_Position = projection * view * vec4(FragPos, 1.0);
               }
          )";

        const char* ps = R"(
                #version 330 core

                in vec3 FragPos;
                in vec3 Normal;

                uniform vec3 lightPos;
                uniform vec3 lightColor;
                uniform vec3 objectColor;

                out vec4 fragment;

                void main()
                {
                    // ambient
                    float ambientStrength = 0.1;
                    vec3  ambient         = ambientStrength * lightColor;

                    // diffuse
                    vec3  norm     = normalize( Normal );                    // 规格化
                    vec3  lightDir = normalize( lightPos - FragPos );        // 
                    float diff     = max( dot ( norm, lightDir ),0.0 );      // 截取最大值
                    vec3  diffuse  = diff * lightColor;                      //
                    vec3  result   = ( ambient + diffuse ) * objectColor;     //

                     fragment = vec4(result, 1.0);                   
                }
          )";

        bool    res = CreateShader(vs, ps);

        if (res)
        {
            m_position    = glGetAttribLocation (m_shaderId, "aPos");
            m_normal      = glGetAttribLocation (m_shaderId, "aNormal");

            m_model       = glGetUniformLocation(m_shaderId, "model");
            m_view        = glGetUniformLocation(m_shaderId, "view");
            m_proj        = glGetUniformLocation(m_shaderId, "projection");

            m_lightPos    = glGetUniformLocation(m_shaderId, "lightPos");
            m_lightColor  = glGetUniformLocation(m_shaderId, "lightColor");
            m_objectColor = glGetUniformLocation(m_shaderId, "objectColor");

        }

        return true;
    }

public:

    unsigned m_position = -1;
    unsigned m_normal = -1;

    unsigned m_model = -1;
    unsigned m_view = -1;
    unsigned m_proj = -1;
    unsigned m_lightPos = -1;
    unsigned m_lightColor = -1;
    unsigned m_objectColor = -1;
};
