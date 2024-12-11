#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "TimeStamp.h"
#include "Camera.h"
class Application
{
public:
	Application();
	virtual ~Application();
	virtual void  Initialize(int width, int height, const char* title);  // 1.初始化
	virtual void  Run();	  
	virtual void  Startup();        // 1.准备数据
	virtual void  Render();         // 2.渲染数据
	virtual void  Shutdown();       // 3.关闭

private:
	// 系统回调函数
	static void  ErrorCallback(int error, const char* description);
	static void  KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void  MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void  CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void  WindowSizeCallback(GLFWwindow* window, int width, int height);
	static void  ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void  WindowIconifyCallback(GLFWwindow* window, int iconified);

	static Application* GetWindow(GLFWwindow* window);
	glm::dvec3 CalcIntersectPoint(Ray& ray); // 射线上的目标点
protected:
	unsigned     CreateTexture(const char* fileName);

protected:
	int           m_winWidth;
	int	          m_winHeight;
	GLFWwindow*   m_pWindow;	   // GLFWwindow窗口
	TimeStamp     m_timeStamp;     // 时间戳
	Camera        m_camera;        // 相机

	double        m_elapsedTime;   // 经过时长
	double        m_frameTime;     // 记录上一帧的时间	,一次SwapBuffers用时		
	unsigned int  m_franerNuber;   // 帧数
	double        m_xPos;		   // 鼠标x
	double        m_yPos;		   // 鼠标y

private:
	bool          m_bRightFlg;	   // 鼠标右键按下 
	glm::dvec2    m_rightPosition; // 鼠标右键按下位置
				  
	bool          m_bLeftFlg;	   // 鼠标左键键按下 
	glm::dvec2    m_leftPosition;  // 鼠标左键按下位置

	 bool         m_isMinimized = false; // 窗口是否最小化
	
};

