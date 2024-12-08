//  定向光shader Directional Light
#pragma once
#include"Shader.h"
class ShaderLightingCastersSpotSoft :public Shader
{
public:
	ShaderLightingCastersSpotSoft() {}
	~ShaderLightingCastersSpotSoft() {}
public:
	virtual bool Initialize()
	{
		const char* vs = R"(

			#version 330 core
			layout (location = 0) in vec3 aPos;
			layout (location = 1) in vec3 aNormal;
			layout (location = 2) in vec2 aTexCoords;
			
			out vec3 FragPos;
			out vec3 Normal;
			out vec2 TexCoords;
			
			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;
			
			void main()
			{
			    FragPos    = vec3(model * vec4(aPos, 1.0));
			    Normal     = mat3(transpose(inverse(model))) * aNormal;  
			    TexCoords  = aTexCoords;
			    
			    gl_Position = projection * view * vec4(FragPos, 1.0);
			}

		)";
		const char* ps = R"(
		
		    #version 330 core
			out vec4 FragColor;
			
			struct Material {
			    sampler2D diffuse;	   // ！！！2D纹理
			    sampler2D specular;    // ！！！2D纹理
			    float     shininess;
			}; 
			
			struct Light {
			    vec3  position;    // 光源位置
				vec3  direction;   // 聚光的方向向量
				float cutOff;      // 聚光半径的切光角
				float outerCutOff; // 外切光角
			   
			    vec3 ambient;
			    vec3 diffuse;
			    vec3 specular;

                // 衰减公式  实现衰减 多项式 常量、一次项和二次项  计算 lightDir
				float constant;
				float linear;
				float quadratic;

			};
			
			in vec3 FragPos;  
			in vec3 Normal;  
			in vec2 TexCoords;
			  
			uniform vec3     viewPos;
			uniform Material material;
			uniform Light    light;
			
			void main()
			{
				
			    // ambient   环境光    
			    vec3 ambient      = light.ambient * vec3(texture(material.diffuse, TexCoords));
			   			    
			    // diffuse 	    
			    vec3  norm        = normalize(Normal);
			    vec3  lightDir    = normalize(light.position - FragPos);  
			    float diff        = max(dot(norm, lightDir), 0.0);
			    vec3  diffuse     = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
			    
			    // specular
			    vec3  viewDir     = normalize(viewPos - FragPos);
			    vec3  reflectDir  = reflect(-lightDir, norm);  
			    float spec        = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
			    vec3  specular    = light.specular * spec * vec3(texture(material.specular, TexCoords));   
			     
                 // spotlight (soft edges) 光边缘柔化
                 float theta = dot(lightDir, normalize(-light.direction)); 
                 float epsilon = (light.cutOff - light.outerCutOff);
                 float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
                 diffuse  *= intensity;
                 specular *= intensity;
    

			     // attenuation 衰减公式
			     float distance    = length(light.position - FragPos);
			     float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
			     
			     ambient  *= attenuation;  
			     diffuse  *= attenuation;
			     specular *= attenuation;   
			   
			     FragColor        =  vec4(ambient + diffuse + specular, 1.0f);  
				
		     } 

		)";

		bool res = CreateShader(vs, ps);

		if (res)
		{
			m_position = glGetAttribLocation(m_shaderId, "aPos");
			m_normal   = glGetAttribLocation(m_shaderId, "aNormal");
			m_uv       = glGetAttribLocation(m_shaderId, "aTexCoords");

			uniform["model"]              = glGetUniformLocation(m_shaderId,"model");
			uniform["view"]               = glGetUniformLocation(m_shaderId,"view");
			uniform["projection"]         = glGetUniformLocation(m_shaderId,"projection");
			uniform["viewPos"]            = glGetUniformLocation(m_shaderId,"viewPos");
			

			uniform["light.position"]     = glGetUniformLocation(m_shaderId, "light.position");
			uniform["light.direction"]    = glGetUniformLocation(m_shaderId, "light.direction");
			uniform["light.cutOff"]       = glGetUniformLocation(m_shaderId, "light.cutOff");
			uniform["light.outerCutOff"]  = glGetUniformLocation(m_shaderId, "light.outerCutOff");
			uniform["light.ambient"]      = glGetUniformLocation(m_shaderId, "light.ambient");
			uniform["light.diffuse"]      = glGetUniformLocation(m_shaderId, "light.diffuse");
			uniform["light.specular"]     = glGetUniformLocation(m_shaderId, "light.specular");

			uniform["light.constant"]     = glGetUniformLocation(m_shaderId, "light.constant");
			uniform["light.linear"]       = glGetUniformLocation(m_shaderId, "light.linear");
			uniform["light.quadratic"]    = glGetUniformLocation(m_shaderId, "light.quadratic");
									      
			
			uniform["material.diffuse"]   = glGetUniformLocation(m_shaderId, "material.diffuse");
			uniform["material.specular"]  = glGetUniformLocation(m_shaderId, "material.specular");
			uniform["material.shininess"] = glGetUniformLocation(m_shaderId, "material.shininess");
		}

		return true;
	}

public:
	unsigned m_position  = -1;
	unsigned m_normal    = -1;
	unsigned m_uv        = -1;

};
