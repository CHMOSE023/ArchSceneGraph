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
				
				layout(location = 0) in vec3 aPosition;   // 顶点位置
				layout(location = 1) in vec3 aNormal;     // 顶点法线
				
				uniform mat4 mvp;                         // 模型视图投影矩阵
				uniform mat4 uModelMatrix;                // 模型矩阵（用于节点变换）
				
				out vec3 fragNormal;                      // 传递给片段着色器的法线
				out vec3 fragPosition;                    // 传递给片段着色器的位置
				
				void main()
				{
				    vec4 worldPosition = uModelMatrix * vec4(aPosition, 1.0);       // 应用模型变换到顶点位置				 
				    fragPosition = worldPosition.xyz;                               // 将变换后的顶点位置和法线传递给片段着色器
				    fragNormal = mat3(transpose(inverse(uModelMatrix))) * aNormal;  // 对法线进行变换
				    gl_Position = mvp * worldPosition;                              // 使用模型视图投影矩阵输出最终变换的位置
				}
          )";


	const char* basePS = R"(
				#version 330 core
				
				in vec3 fragNormal;             // 从顶点着色器传递的法线
				in vec3 fragPosition;           // 从顶点着色器传递的位置
				
				uniform vec3  uLightPosition;   // 世界空间中的光源位置
				uniform vec4  uBaseColorFactor; // 基础颜色（来自材质）
				uniform float uMetallicFactor;  // 金属因子（此材质为 0.0）
				
				out vec4 FragColor;	
			
				void main()
				{
				    vec3 lightDir     = normalize(uLightPosition - fragPosition);  // 简单的漫反射光照模型
				    float diff        = max(dot(fragNormal, lightDir), 0.0);				  
				    vec3 diffuseColor = uBaseColorFactor.rgb * diff;               // 应用基础颜色				 
				    FragColor         = vec4(diffuseColor, uBaseColorFactor.a);    // 输出最终颜色
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


	// 1.准备数据
	virtual void Startup()
	{ 
		if (!ReadGLTF(R"(E:\glTF-Sample-Models-main\2.0\Box\glTF-Embedded\Box.gltf)")) 
			return;

		pshader = new Shader();
		pshader->CreateShader(baseVS,basePS);
		  
		// 读取网格数据
		for (size_t i = 0; i < m_model.meshes.size(); i++)
		{
			 Mesh* mesh = new  Mesh(pshader);
			 
			 mesh->LoadMesh(m_model, m_model.meshes[i]);
			 
			 m_sceneNode.AddChild(mesh);
		}
		 
		
	}

	// 2.渲染数据
	virtual void Render()
	{
		m_ground.Render(m_camera);
		 
		m_sceneNode.Render(m_camera);
	}

	// 3.关闭
	virtual void Shutdown() 
	{
		std::cout << "关闭" << std::endl;
	}

private:
	tinygltf::Model  m_model;
	Shader  *  pshader;
	Shader     m_shader; 
	SceneNode  m_sceneNode;
	// 模型类
	Ground    m_ground;   // 地面

};
 


int main()
{     
	WinGLTF winGLTF;
	winGLTF.Initialize(1920, 1080, APPLICTION);
	winGLTF.Run();     
}
