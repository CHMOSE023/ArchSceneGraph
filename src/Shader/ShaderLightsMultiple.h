//  定向光shader Directional Light
#pragma once
#include <vector>
#include <string>
#include"Shader.h"
class ShaderLightsMultiple :public Shader
{
public:
	ShaderLightsMultiple() {}
	~ShaderLightsMultiple() {}
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
		
			// 材质
			struct Material {
			    sampler2D diffuse;	   // ！！！2D纹理
			    sampler2D specular;    // ！！！2D纹理
			    float     shininess;
			}; 
            
            //直射光
			struct DirLight {
			    vec3 direction;
				
			    vec3 ambient;
			    vec3 diffuse;
			    vec3 specular;
			};

			// 点光源
			struct PointLight {
			    vec3 position;
			    
			    float constant;
			    float linear;
			    float quadratic;
				
			    vec3 ambient;
			    vec3 diffuse;
			    vec3 specular;
			};	
		
            // 投射光源
			struct SpotLight  {
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

			// 点光源数量
			#define NR_POINT_LIGHTS 4 

			in vec3 FragPos;  
			in vec3 Normal;  
			in vec2 TexCoords;
			  
			uniform vec3       viewPos;
			uniform DirLight   dirLight;
			uniform PointLight pointLights[NR_POINT_LIGHTS];
			uniform SpotLight  spotLight;
			uniform Material   material;

			// 灯光计算函数
			vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
			vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
			vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
			
			out vec4 FragColor;
			void main()
			{
				// properties
				vec3 norm    = normalize(Normal);
				vec3 viewDir = normalize(viewPos - FragPos);
				vec3 result  = CalcDirLight(dirLight, norm, viewDir);

				for(int i = 0; i < NR_POINT_LIGHTS; i++)
				{
				    result += CalcPointLight(pointLights[i], norm, FragPos, viewDir); 
				}

				result   += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
			    FragColor =  vec4(result, 1.0f);  
				
		     } 


			// calculates the color when using a directional light.
			vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
			{
			    vec3 lightDir = normalize(-light.direction);

			    // diffuse shading
			    float diff = max(dot(normal, lightDir), 0.0);
			    
				// specular shading
			    vec3 reflectDir = reflect(-lightDir, normal);
			    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

			    // combine results
			    vec3 ambient    = light.ambient * vec3(texture(material.diffuse, TexCoords));
			    vec3 diffuse    = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
			    vec3 specular   = light.specular * spec * vec3(texture(material.specular, TexCoords));
			    return (ambient + diffuse + specular);
			}
			
			// calculates the color when using a point light.
			vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
			{
			    vec3 lightDir = normalize(light.position - fragPos);

			    // diffuse shading
			    float diff = max(dot(normal, lightDir), 0.0);

			    // specular shading
			    vec3 reflectDir = reflect(-lightDir, normal);
			    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

			    // attenuation
			    float distance    = length(light.position - fragPos);
			    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
			    // combine results
			    vec3 ambient   = light.ambient * vec3(texture(material.diffuse, TexCoords));
			    vec3 diffuse   = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
			    vec3 specular  = light.specular * spec * vec3(texture(material.specular, TexCoords));

			    ambient  *= attenuation;
			    diffuse  *= attenuation;
			    specular *= attenuation;
			    return (ambient + diffuse + specular);
			}

			// calculates the color when using a spot light.
			vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
			{
				vec3 lightDir = normalize(light.position - fragPos);
			    
				// diffuse shading
			    float diff = max(dot(normal, lightDir), 0.0);
			    
				// specular shading
			    vec3 reflectDir = reflect(-lightDir, normal);
			    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
			   
				 // attenuation
			    float distance    = length(light.position - fragPos);
			    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
			    
				// spotlight intensity
			    float theta     = dot(lightDir, normalize(-light.direction)); 
			    float epsilon   = light.cutOff - light.outerCutOff;
			    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
			   
				 // combine results
			    vec3 ambient   = light.ambient * vec3(texture(material.diffuse, TexCoords));
			    vec3 diffuse   = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
			    vec3 specular  = light.specular * spec * vec3(texture(material.specular, TexCoords));
			    
				ambient  *= attenuation * intensity;
			    diffuse  *= attenuation * intensity;
			    specular *= attenuation * intensity;
			    
				return (ambient + diffuse + specular);
			
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
			
			// 直射光
			uniform["dirLight.direction"]     = glGetUniformLocation(m_shaderId, "dirLight.direction");
			uniform["dirLight.ambient"]       = glGetUniformLocation(m_shaderId, "dirLight.ambient");
			uniform["dirLight.diffuse"]       = glGetUniformLocation(m_shaderId, "dirLight.diffuse");
			uniform["dirLight.specular"]      = glGetUniformLocation(m_shaderId, "dirLight.specular");
			
			// 投射光
			uniform["spotLight.position"]     = glGetUniformLocation(m_shaderId, "spotLight.position");
			uniform["spotLight.direction"]    = glGetUniformLocation(m_shaderId, "spotLight.direction");
			uniform["spotLight.cutOff"]       = glGetUniformLocation(m_shaderId, "spotLight.cutOff");
			uniform["spotLight.outerCutOff"]  = glGetUniformLocation(m_shaderId, "spotLight.outerCutOff");

			uniform["spotLight.constant"]     = glGetUniformLocation(m_shaderId, "spotLight.constant");
			uniform["spotLight.linear"]       = glGetUniformLocation(m_shaderId, "spotLight.linear");
			uniform["spotLight.quadratic"]    = glGetUniformLocation(m_shaderId, "spotLight.quadratic");
			
			uniform["spotLight.ambient"]      = glGetUniformLocation(m_shaderId, "spotLight.ambient");
			uniform["spotLight.diffuse"]      = glGetUniformLocation(m_shaderId, "spotLight.diffuse");
			uniform["spotLight.specular"]     = glGetUniformLocation(m_shaderId, "spotLight.specular");
									      
			// 材质
			uniform["material.diffuse"]       = glGetUniformLocation(m_shaderId, "material.diffuse");
			uniform["material.specular"]      = glGetUniformLocation(m_shaderId, "material.specular");
			uniform["material.shininess"]     = glGetUniformLocation(m_shaderId, "material.shininess");


			std::vector<std::string> pointes = {
				"pointLights[0].",
				"pointLights[1].",
				"pointLights[2].",
				"pointLights[3].",
			};		

			for (std::string point : pointes)
			{
				// 点光源
				uniform[point + "position"]  = glGetUniformLocation(m_shaderId, (point + "position").c_str());
				uniform[point + "ambient"]   = glGetUniformLocation(m_shaderId, (point + "ambient").c_str());
				uniform[point + "diffuse"]   = glGetUniformLocation(m_shaderId, (point + "diffuse").c_str());
				uniform[point + "specular"]  = glGetUniformLocation(m_shaderId, (point + "specular").c_str());
				uniform[point + "constant"]  = glGetUniformLocation(m_shaderId, (point + "constant").c_str());
				uniform[point + "linear"]    = glGetUniformLocation(m_shaderId, (point + "linear").c_str());
				uniform[point + "quadratic"] = glGetUniformLocation(m_shaderId, (point + "quadratic").c_str());
			}
		}

		return true;
	}

public:
	unsigned m_position  = -1;
	unsigned m_normal    = -1;
	unsigned m_uv        = -1;

};
