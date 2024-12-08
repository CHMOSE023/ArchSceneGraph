#pragma once
#include"Shader.h"
class ShaderLightingMaps :public Shader
{
public:
	ShaderLightingMaps() {}
	~ShaderLightingMaps() {}
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
			    sampler2D diffuse;	   // £¡£¡£¡2DÎÆÀí
			    sampler2D specular;    // £¡£¡£¡2DÎÆÀí
			    float     shininess;
			}; 
			
			struct Light {
			    vec3 position;
			
			    vec3 ambient;
			    vec3 diffuse;
			    vec3 specular;
			};
			
			in vec3 FragPos;  
			in vec3 Normal;  
			in vec2 TexCoords;
			  
			uniform vec3     viewPos;
			uniform Material material;
			uniform Light    light;
			
			void main()
			{
			    // ambient
			    vec3 ambient    = light.ambient * texture(material.diffuse, TexCoords).rgb;
			  				    
			    // diffuse 	    
			    vec3 norm        = normalize(Normal);
			    vec3 lightDir    = normalize(light.position - FragPos);
			    float diff       = max(dot(norm, lightDir), 0.0);
			    vec3 diffuse     = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
			    
			    // specular
			    vec3 viewDir     = normalize(viewPos - FragPos);
			    vec3 reflectDir  = reflect(-lightDir, norm);  
			    float spec       = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
			    vec3 specular    = light.specular * spec * texture(material.specular, TexCoords).rgb;  
			        
			    vec3 result      = ambient + diffuse + specular;
			    FragColor        = vec4(result, 1.0);
			} 

		)";

		bool res = CreateShader(vs, ps);

		if (res)
		{
			m_position = glGetAttribLocation(m_shaderId, "aPos");
			m_normal   = glGetAttribLocation(m_shaderId, "aNormal");
			m_uv       = glGetAttribLocation(m_shaderId, "aTexCoords");

			uniform["model"]      = glGetUniformLocation(m_shaderId,"model");
			uniform["view"]       = glGetUniformLocation(m_shaderId,"view");
			uniform["projection"] = glGetUniformLocation(m_shaderId,"projection");

			uniform["viewPos"]    = glGetUniformLocation(m_shaderId,"viewPos");

			uniform["light"]      = glGetUniformLocation(m_shaderId,"light");

			uniform["light.position"]     = glGetUniformLocation(m_shaderId, "light.position");
			uniform["light.ambient"]      = glGetUniformLocation(m_shaderId, "light.ambient");
			uniform["light.diffuse"]      = glGetUniformLocation(m_shaderId, "light.diffuse");
			uniform["light.specular"]     = glGetUniformLocation(m_shaderId, "light.specular");
									      
			
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
