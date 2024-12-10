#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"
#define APPLICTION "WinGLTF"
#include "../Core/Application.h" 
#include "../Core/SceneNode.h" 
#include <iostream>
#include "../Shader/Shader.h"
#include "Mesh.h"
#include "../Entity/Ground.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

static std::string GetFilePathExtension(const std::string& FileName) {
	if (FileName.find_last_of(".") != std::string::npos)
		return FileName.substr(FileName.find_last_of(".") + 1);
	return "";
}

typedef struct
{
	std::map<std::string, GLint> attribs;
	std::map<std::string, GLint> uniforms;
} GLProgramState;

typedef struct 
{
	GLuint vb;
} GLBufferState;

GLProgramState gGLProgramState;

std::map<int, GLBufferState> gBufferState;

class WinGLTF:public Application
{ 


	const char* baseVS = R"(
				#version 330 core
				
				layout(location = 0) in vec3 aPosition;   // ����λ��
				layout(location = 1) in vec3 aNormal;     // ���㷨��
				
				uniform mat4 mvp;                         // ģ����ͼͶӰ����
				uniform mat4 uModelMatrix;                // ģ�;������ڽڵ�任��
				
				out vec3 fragNormal;                      // ���ݸ�Ƭ����ɫ���ķ���
				out vec3 fragPosition;                    // ���ݸ�Ƭ����ɫ����λ��
				
				void main()
				{
				    vec4 worldPosition = uModelMatrix * vec4(aPosition, 1.0);       // Ӧ��ģ�ͱ任������λ��				 
				    fragPosition = worldPosition.xyz;                               // ���任��Ķ���λ�úͷ��ߴ��ݸ�Ƭ����ɫ��
				    fragNormal = mat3(transpose(inverse(uModelMatrix))) * aNormal;  // �Է��߽��б任
				    gl_Position = mvp * worldPosition;                              // ʹ��ģ����ͼͶӰ����������ձ任��λ��
				}
          )";


	const char* basePS = R"(
				#version 330 core
				
				in vec3 fragNormal;             // �Ӷ�����ɫ�����ݵķ���
				in vec3 fragPosition;           // �Ӷ�����ɫ�����ݵ�λ��
				
				uniform vec3  uLightPosition;   // ����ռ��еĹ�Դλ��
				uniform vec4  uBaseColorFactor; // ������ɫ�����Բ��ʣ�
				uniform float uMetallicFactor;  // �������ӣ��˲���Ϊ 0.0��
				
				out vec4 FragColor;	
			
				void main()
				{
				    vec3 lightDir     = normalize(uLightPosition - fragPosition);  // �򵥵����������ģ��
				    float diff        = max(dot(fragNormal, lightDir), 0.0);				  
				    vec3 diffuseColor = uBaseColorFactor.rgb * diff;               // Ӧ�û�����ɫ				 
				    FragColor         = vec4(diffuseColor, uBaseColorFactor.a);    // ���������ɫ
				}
          )";


	bool ReadGLTF(std::string input_filename)
	{ 
		auto texture = CreateTexture("textures/ground.jpg");
		m_ground.Init();
		m_ground.SetTexture(texture);



		tinygltf::TinyGLTF gltf_ctx;
		std::string err;
		std::string warn;
		std::string ext = GetFilePathExtension(input_filename);

		bool ret = false;
		if (ext.compare("glb") == 0)
		{
			std::cout << "Reading binary glTF: " << input_filename << std::endl;
			ret = gltf_ctx.LoadBinaryFromFile(&m_model, &err, &warn, input_filename.c_str()); // assume binary glTF.
		}
		else
		{
			std::cout << "Reading ASCII glTF: " << input_filename << std::endl;
			ret = gltf_ctx.LoadASCIIFromFile(&m_model, &err, &warn, input_filename.c_str()); // assume ascii glTF.				
		}

		if (!warn.empty()) {
			printf("Warn: %s\n", warn.c_str());
		}

		if (!err.empty()) {
			printf("ERR: %s\n", err.c_str());
		}

		return ret;
	}


	// 1.׼������
	virtual void Startup()
	{ 
		if (!ReadGLTF(R"(E:\glTF-Sample-Models-main\2.0\Box\glTF-Embedded\Box.gltf)")) 
			return;

		pshader = new Shader();
		pshader->CreateShader(baseVS,basePS);
		  
		// ��ȡ��������
		for (size_t i = 0; i < m_model.meshes.size(); i++)
		{
			 Mesh* mesh = new  Mesh(pshader);
			 
			 mesh->LoadMesh(m_model, m_model.meshes[i]);
			 
			 m_sceneNode.AddChild(mesh);
		}
		 
		
	}

	// 2.��Ⱦ����
	virtual void Render()
	{
		m_ground.Render(m_camera);
		 
		m_sceneNode.Render(m_camera);
	}

	// 3.�ر�
	virtual void Shutdown() 
	{
		std::cout << "�ر�" << std::endl;
	}

private:
	tinygltf::Model  m_model;
	Shader  *  pshader;
	Shader     m_shader; 
	SceneNode  m_sceneNode;
	// ģ����
	Ground    m_ground;   // ����

};
 


int main()
{     
	WinGLTF winGLTF;
	winGLTF.Initialize(1920, 1080, APPLICTION);
	winGLTF.Run();     
}
