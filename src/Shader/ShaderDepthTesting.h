#pragma once 
#include "Shader.h"

class ShaderDepthTesting:public Shader
{
public:
	virtual bool Initialize()
	{
		const char* vs = R"(
			#version 330 core
			layout (location = 0) in vec3 aPos;
			layout (location = 1) in vec2 aTexCoords;
			
			out vec2 TexCoords;
			
			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;
			
			void main()
			{
			    TexCoords = aTexCoords;    
			    gl_Position = projection * view * model * vec4(aPos, 1.0);
			}

		)";
		const char* ps = R"(
			#version 330 core
			out vec4 FragColor;
			
			in vec2 TexCoords;
			
			uniform sampler2D texture1;


			float LinearizeDepth(float depth)
			{
			    float near = 0.1;
			    float far = 100.0;
			    float z = depth * 2.0 - 1.0; // Back to NDC
			    return (2.0 * near) / (far + near - z * (far - near));
			}

			
			void main()
			{    
			   
                //float depth = LinearizeDepth(gl_FragCoord.z);
			    //FragColor = vec4(vec3(depth), 1.0f);;

			     FragColor = texture(texture1, TexCoords);
			}

		)";

		bool res = CreateShader(vs, ps);

		if (res)
		{
			m_position            = glGetAttribLocation(m_shaderId, "aPos");		
			m_uv                  = glGetAttribLocation(m_shaderId, "aTexCoords");

			uniform["model"]      = glGetUniformLocation(m_shaderId, "model");
			uniform["view"]       = glGetUniformLocation(m_shaderId, "view");
			uniform["projection"] = glGetUniformLocation(m_shaderId, "projection");
			
			uniform["texture1"]   = glGetUniformLocation(m_shaderId,  "texture1");
			
		}
		return true;
	}

public:
	unsigned m_position = -1;
	unsigned m_uv = -1;



};

