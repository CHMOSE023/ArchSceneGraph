#include "../Core/Application.h"
#include "../Core/MeshNode.h"
#include "../Entity/Ground.h"
#include "../Entity/Triangle.h"
#include "../Entity/Box.h"
#include "../Entity/BoxColor.h"
#include "iostream"
#include <vector>
#include <memory>
#include "../Entity/BoxBasicLighting.h"

#define APPLICTION "WinApp"
 
class WinApp :public Application
{


	// 1.׼������
	virtual void Startup()
	{
		// ��������ڵ� 

		m_textureCity   = CreateTexture("textures/chongqing.jpg");
		m_textureGround = CreateTexture("textures/ground.jpg");

		m_triangle.Init();
		m_box.Init();
		m_boxColor.Init();

		m_ground.Init();
		m_ground.SetTexture(m_textureGround);

		m_boxBasicLighting.Init();
	
#pragma region baseShader
 
		const char* baseVS = R"(
         #version 330 core
         layout(location = 0) in vec3 position;
         layout(location = 1) in vec3 color;
         out vec3 vertexColor;
         uniform mat4 mvp; 
         void main() 
         {
            gl_Position = mvp * vec4(position, 1.0);
            vertexColor = color;
         };
        )";
		
		const char* basePS = R"(
         #version 330 core  
         in  vec3 vertexColor;  
         out vec4 color;  
         void main()
         {  
            color = vec4(vertexColor, 1.0);  
         };
        )";
		
		Shader* baseShader = new Shader();
		baseShader->CreateShader(baseVS,basePS);
		baseShader->uniform["mvp"] = glGetUniformLocation(baseShader->m_shaderId, "mvp");

#pragma endregion	



#pragma region  ��������1

		std::vector<GLfloat> vertices = {
			// ��������           // ��ɫ
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // ���½� (��)
			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // ���½� (��)
			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // ���Ͻ� (��)
			-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // ���Ͻ� (��)
			-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, // ���½� (Ʒ��)
			 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // ���½� (��)
			 0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.5f, // ���Ͻ� (��)
			-0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.5f  // ���Ͻ� (��)
		};

		std::vector<GLuint> indices = {
			0, 1, 2,
			0, 2, 3,
			1, 5, 6,
			1, 6, 2,
			5, 4, 7,
			5, 7, 6,
			4, 0, 3,
			4, 3, 7,
			3, 2, 6,
			3, 6, 7,
			4, 5, 1,
			4, 1, 0
		};

		pMeshNode = new MeshNode(baseShader);
		pMeshNode->LoadMesh(vertices, indices);

		pMeshNode1 = new MeshNode(baseShader);
		pMeshNode1->LoadMesh(vertices, indices);

		m_RootNode.AddChild(pMeshNode);
		m_RootNode.AddChild(pMeshNode1);

#pragma endregion	



#pragma region  lightShader	

		const char* lightVS = R"(
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
                
                // Normal ���߾���  ���Դ�cpu����ô��롣
                Normal  = mat3(transpose(inverse(model))) * aNormal; // 3��3��������֤��ʧȥ��λ�������Լ��ܹ�����vec3�ķ�����
                
                gl_Position = projection * view * vec4(FragPos, 1.0);
            }
          )";

		const char* lightPS = R"(
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
                // ambient �������� Ambient Lighting

                float ambientStrength = 0.6;
                vec3  ambient         = ambientStrength * lightColor;               

                // diffuse  ��������� Diffuse Lighting

                vec3  norm     = normalize(Normal);               // ��������׼��
                vec3  lightDir = normalize(lightPos - FragPos);   // ��׼�� ����������
                float diff     = max(dot(norm, lightDir), 0.0);   // norm��lightDir�������е�� ����������֮��ĽǶ�Խ������������ͻ�ԽС
                vec3  diffuse  = diff * lightColor;               // ������ȱ�ٷ��߾������ ���κ�ʧ��
                
                // specular �������(Specular Lighting)

                float specularStrength = 0.5;                    // ����ǿ��(Specular Intensity)����

                vec3  viewDir    = normalize(viewPos - FragPos); // �������߷�������
                vec3  reflectDir = reflect(-lightDir, norm);     // ������ķ�������
                float spec       = pow(max(dot(viewDir, reflectDir), 0.0), 50); 
                vec3  specular   = specularStrength * spec * lightColor;  
                    
                vec3 result = (ambient + diffuse + specular) * objectColor;
                FragColor = vec4(result, 1.0);
            } 
          )";

		Shader* lightShader = new Shader();
		lightShader->CreateShader(lightVS, lightPS);

		lightShader->uniform["aPos"]        = glGetAttribLocation(lightShader->m_shaderId, "aPos");
		lightShader->uniform["aNormal"]     = glGetAttribLocation(lightShader->m_shaderId, "aNormal");

		lightShader->uniform["model"]       = glGetUniformLocation(lightShader->m_shaderId, "model");
		lightShader->uniform["view"]        = glGetUniformLocation(lightShader->m_shaderId, "view");
		lightShader->uniform["projection"]  = glGetUniformLocation(lightShader->m_shaderId, "projection");

		lightShader->uniform["lightPos"]    = glGetUniformLocation(lightShader->m_shaderId, "lightPos");
		lightShader->uniform["viewPos"]     = glGetUniformLocation(lightShader->m_shaderId, "viewPos");
		lightShader->uniform["lightColor"]  = glGetUniformLocation(lightShader->m_shaderId, "lightColor");
		lightShader->uniform["objectColor"] = glGetUniformLocation(lightShader->m_shaderId, "objectColor");

#pragma endregion	

#pragma region  �ƹ����

		std::vector<GLfloat> normalVertices = 
		{
		 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		 -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		  0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		 -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		 -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};

		pLightMeshNode1 = new MeshNode(lightShader);
		pLightMeshNode1->LoadMesh(normalVertices);

		m_RootNode.AddChild(pLightMeshNode1);

#pragma endregion	

	 
	} 
	

	void UpdataRootNode()
	{
		/*����*/
		GLfloat currentTime = glfwGetTime();
		glm::mat4 model(1.0f);
		model = glm::mat4(1);		
		model = glm::translate(model, glm::vec3(-3.0, 0, 0)); // X��ƽ��3����λ
		model = glm::rotate(model, glm::radians(50.0f) * currentTime, glm::vec3(0.5f, 1.0f, 0.0f));

		pMeshNode->SetLocalTransform(model);

		/*����1*/
		glm::mat4 model1(1.0f);
		model1 = glm::mat4(1);	
		model1 = glm::translate(model1, glm::vec3(3.0, 0, 0)); // X��ƽ��3����λ
		model1 = glm::rotate(model1, glm::radians(100.0f) * currentTime, glm::vec3(0.0f, 1.0f, 0.0f));

		pMeshNode1->SetLocalTransform(model1);

		/*����2*/
		glm::mat4 model2(1.0f);
		model2 = glm::mat4(1);
		model2 = glm::rotate(model2, glm::radians(100.0f) * currentTime, glm::vec3(0.0f, 1.0f, 0.0f));

		pLightMeshNode1->SetLocalTransform(model2);


	}

	// 2.��Ⱦ����
	virtual void Render()
	{ 
		m_ground.Render(m_camera);
	    m_triangle.Render(m_camera);
	    // m_boxColor.Render(m_camera);
	    // m_box.Render(m_camera);

		//m_boxBasicLighting.Render(m_camera);

		UpdataRootNode();		     // ��������
		m_RootNode.Render(m_camera); // ��Ⱦ����
	}

	// 3.�ر�
	virtual void Shutdown()
	{
		// ɾ������
		glDeleteTextures(1, &m_textureCity);
		glDeleteTextures(1, &m_textureGround);

		std::cout << "Shutdown" << std::endl;
	}

private: // ����	

	// ģ����
	Ground    m_ground;   // ����
	Triangle  m_triangle; // ������
	Box       m_box;      // ��ɫ����
	BoxColor  m_boxColor; // ����ԭʼ��ɫ

	SceneNode m_RootNode; // ������

	BoxBasicLighting  m_boxBasicLighting; // �������հ���

	unsigned  m_textureCity = 0;
	unsigned  m_textureGround = 0;
	MeshNode* pMeshNode =nullptr;
	MeshNode* pMeshNode1 =nullptr;
	MeshNode* pLightMeshNode1 =nullptr;
	 
};

int main()
{  
	WinApp winApp;
	winApp.Initialize(1920, 1080, APPLICTION);
	winApp.Run();
}
