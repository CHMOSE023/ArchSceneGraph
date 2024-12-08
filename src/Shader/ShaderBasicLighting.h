#pragma once 
#include  "../Shader/Shader.h"
#include  <map>
#include <string>
// 基础关照
class ShaderBasicLighting : public Shader
{
public:
    ShaderBasicLighting() {};
    ~ShaderBasicLighting() {};

    virtual bool  Initialize()
    {
        const char* vs = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aNormal;
            
            out vec3 FragPos;
            out vec3 Normal;
            
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;
            
            void main()
            {
                FragPos = vec3(model * vec4(aPos, 1.0));
                
                // Normal 法线矩阵  可以从cpu计算好传入。
                Normal  = mat3(transpose(inverse(model))) * aNormal; // 3×3矩阵，来保证它失去了位移属性以及能够乘以vec3的法向量
                
                gl_Position = projection * view * vec4(FragPos, 1.0);
            }
          )";

        const char* ps = R"(
            #version 330 core
            out vec4 FragColor;
            
            in vec3 Normal;  
            in vec3 FragPos;  
              
            uniform vec3 lightPos; 
            uniform vec3 viewPos; 
            uniform vec3 lightColor;
            uniform vec3 objectColor;
            
            void main()
            {
                // ambient 环境光照 Ambient Lighting

                float ambientStrength = 0.1;
                vec3  ambient         = ambientStrength * lightColor;               

                // diffuse  漫反射光照 Diffuse Lighting

                vec3  norm     = normalize(Normal);               // 法向量标准化
                vec3  lightDir = normalize(lightPos - FragPos);   // 标准化 计算向量差
                float diff     = max(dot(norm, lightDir), 0.0);   // norm和lightDir向量进行点乘 ，两个向量之间的角度越大，漫反射分量就会越小
                vec3  diffuse  = diff * lightColor;               // ！！！缺少法线矩阵计算 变形后失真
                
                // specular 镜面光照(Specular Lighting)

                float specularStrength = 0.5;                    // 镜面强度(Specular Intensity)变量

                vec3  viewDir    = normalize(viewPos - FragPos); // 计算视线方向向量
                vec3  reflectDir = reflect(-lightDir, norm);     // 法线轴的反射向量
                float spec       = pow(max(dot(viewDir, reflectDir), 0.0), 50); 
                vec3  specular   = specularStrength * spec * lightColor;  
                    
                vec3 result = (ambient + diffuse + specular) * objectColor;
                FragColor = vec4(result, 1.0);
            } 
          )";

        bool    res = CreateShader(vs, ps);

        if (res)
        {
            m_position   = glGetAttribLocation(m_shaderId, "aPos");
            m_normal     = glGetAttribLocation(m_shaderId, "aNormal");


            m_model      = glGetUniformLocation(m_shaderId, "model");
            m_view       = glGetUniformLocation(m_shaderId, "view");
            m_projection = glGetUniformLocation(m_shaderId, "projection");

            m_lightPos     = glGetUniformLocation(m_shaderId, "lightPos");
            m_viewPos      = glGetUniformLocation(m_shaderId, "viewPos");          
            m_lightColor   = glGetUniformLocation(m_shaderId, "lightColor");
            m_objectColor  = glGetUniformLocation(m_shaderId, "objectColor");

            uniform["model"]       = m_model;
            uniform["view"]        = m_view;
            uniform["projection"]  = m_projection;

            uniform["lightPos"]    = m_lightPos;
            uniform["viewPos"]     = m_viewPos;
            uniform["lightColor"]  = m_lightColor;
            uniform["objectColor"] = m_objectColor;

        }
        return true;
    }

public:
    unsigned m_position    = -1;
    unsigned m_normal      = -1;

private:
    unsigned m_model       = -1;
    unsigned m_view        = -1;
    unsigned m_projection  = -1;

    unsigned m_lightPos    = -1;
    unsigned m_viewPos     = -1;   
    unsigned m_lightColor  = -1;
    unsigned m_objectColor = -1;

};

