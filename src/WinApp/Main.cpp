#include "../Core/Application.h"
#include "../Core/MeshNode.h"
#include "../Entity/Ground.h"
#include "../Entity/Triangle.h"
#include "../Entity/Box.h"
#include "../Entity/BoxColor.h"
#include "iostream"
#include <vector>
#include <memory>

#define APPLICTION "WinApp"
 
class WinApp :public Application
{

	// 1.准备数据
	virtual void Startup()
	{
		// 创建网格节点 

		m_textureCity   = CreateTexture("textures/chongqing.jpg");
		m_textureGround = CreateTexture("textures/ground.jpg");

		m_triangle.Init();
		m_box.Init();
		m_boxColor.Init();

		m_ground.Init();
		m_ground.SetTexture(m_textureGround);
	
#pragma region 场景树节点
 
		const char* vs = R"(
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
		
		const char* ps = R"(
         #version 330 core  
         in  vec3 vertexColor;  
         out vec4 color;  
         void main()
         {  
            color = vec4(vertexColor, 1.0);  
         };
        )";
		
		Shader* shader = new Shader();

		shader->CreateShader(vs,ps);
		shader->uniform["mvp"] = glGetUniformLocation(shader->m_shaderId, "mvp");
		
		pMeshNode  = new MeshNode("boxVao", shader);
		pMeshNode1 = new MeshNode("boxVao", shader);

		m_RootNode.AddChild(pMeshNode);
		m_RootNode.AddChild(pMeshNode1);
		  
#pragma endregion	 

		std::cout << "Startup" << std::endl;
	} 
	

	void UpdataRootNode()
	{
		/*盒子*/
		GLfloat currentTime = glfwGetTime();
		glm::mat4 model(1.0f);
		model = glm::mat4(1);		 
		model = glm::rotate(model, glm::radians(50.0f) * currentTime, glm::vec3(0.5f, 1.0f, 0.0f));

		pMeshNode->SetLocalTransform(model);

		/*盒子1*/
		glm::mat4 model1(1.0f);
		model1 = glm::mat4(1);
		model1 = glm::translate(model1, glm::vec3(3.0, 0, 0)); // X轴平移3个单位
		model1 = glm::rotate(model1, glm::radians(100.0f) * currentTime, glm::vec3(0.0f, 1.0f, 0.0f));

		pMeshNode1->SetLocalTransform(model1);


	}

	// 2.渲染数据
	virtual void Render()
	{ 
		m_ground.Render(m_camera);
	    m_triangle.Render(m_camera);
	    // m_boxColor.Render(m_camera);
	    // m_box.Render(m_camera);

		UpdataRootNode();		     // 更新数据
		m_RootNode.Render(m_camera); // 渲染盒子
	}

	// 3.关闭
	virtual void Shutdown()
	{
		// 删除纹理
		glDeleteTextures(1, &m_textureCity);
		glDeleteTextures(1, &m_textureGround);

		std::cout << "Shutdown" << std::endl;
	}

private: // 变量	

	// 模型类
	Ground    m_ground;   // 地面
	Triangle  m_triangle; // 三角形
	Box       m_box;      // 彩色盒子
	BoxColor  m_boxColor; // 物体原始颜色

	SceneNode m_RootNode; // 场景树

	unsigned  m_textureCity = 0;
	unsigned  m_textureGround = 0;
	MeshNode* pMeshNode =nullptr;
	MeshNode* pMeshNode1 =nullptr;
};

int main()
{  
	WinApp winApp;
	winApp.Initialize(1920, 1080, APPLICTION);
	winApp.Run();
}
