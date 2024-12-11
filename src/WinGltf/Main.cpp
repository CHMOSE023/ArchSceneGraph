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


	const char* basePS = R"(
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

                float ambientStrength = 0.6;
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
	    pshader->uniform["aPos"]        = glGetAttribLocation(pshader->m_shaderId, "aPos");
		pshader->uniform["aNormal"]     = glGetAttribLocation(pshader->m_shaderId, "aNormal");

		pshader->uniform["model"]       = glGetUniformLocation(pshader->m_shaderId, "model");
		pshader->uniform["view"]        = glGetUniformLocation(pshader->m_shaderId, "view");
		pshader->uniform["projection"]  = glGetUniformLocation(pshader->m_shaderId, "projection");

		pshader->uniform["lightPos"]    = glGetUniformLocation(pshader->m_shaderId, "lightPos");
		pshader->uniform["viewPos"]     = glGetUniformLocation(pshader->m_shaderId, "viewPos");
		pshader->uniform["lightColor"]  = glGetUniformLocation(pshader->m_shaderId, "lightColor");
		pshader->uniform["objectColor"] = glGetUniformLocation(pshader->m_shaderId, "objectColor");  


		// 读取网格数据

		Mesh* mesh = new  Mesh(pshader);

		mesh->LoadMesh(m_model, m_model.meshes[0]);

		m_sceneNode.AddChild(mesh);
		 
		
	}

	// 2.渲染数据
	virtual void Render()
	{
		// m_ground.Render(m_camera);
		 
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
