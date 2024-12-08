#include "../Core/Application.h"
#include "../Entity/Ground.h"
#include "../Entity/Triangle.h"
#include "../Entity/Box.h"
#include "../Entity/BoxColor.h"
#include "iostream"

#define APPLICTION "WinApp"

class WinApp :public Application
{

	// 1.准备数据
	virtual void Startup()
	{
		m_textureCity = CreateTexture("textures/chongqing.jpg");
		m_textureGround = CreateTexture("textures/ground.jpg");

		m_triangle.Init();
		m_box.Init();
		m_boxColor.Init();

		m_ground.Init();
		m_ground.SetTexture(m_textureGround);

	}


	// 2.渲染数据
	virtual void Render()
	{

		m_ground.Render(m_camera);
		m_triangle.Render(m_camera);

		m_boxColor.Render(m_camera);
		m_box.Render(m_camera);


	}

	// 3.关闭
	virtual void Shutdown()
	{
		// 删除纹理
		glDeleteTextures(1, &m_textureCity);
		glDeleteTextures(1, &m_textureGround);
	}

private: // 变量	

	// 模型类
	Ground    m_ground;   // 地面
	Triangle  m_triangle; // 三角形
	Box       m_box;      // 彩色盒子
	BoxColor  m_boxColor; // 物体原始颜色

	unsigned  m_textureCity = 0;
	unsigned  m_textureGround = 0;

};

int main()
{
	WinApp winApp;
	winApp.Initialize(1920, 1080, APPLICTION);
	winApp.Run();
}
