#include "Application.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

#pragma region glfw窗口及事件处理
Application::Application()
{
	m_bRightFlg = false;
	m_bLeftFlg  = false;
}

Application::~Application()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void Application::ErrorCallback(int error, const char* description)
{

}

// 键盘消息
void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

// 鼠标按下消息
void Application::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Application* app = GetWindow(window);

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		app->m_bLeftFlg = true;
		app->m_leftPosition = glm::dvec2(app->m_xPos, app->m_yPos);
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		app->m_bLeftFlg = false;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		app->m_bRightFlg = true;
		app->m_rightPosition = glm::dvec2(app->m_xPos, app->m_yPos);
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		app->m_bRightFlg = false;
	}
}

// 射线上的目标点
glm::dvec3 Application::CalcIntersectPoint(Ray& ray)
{
	glm::dvec3    pos = ray.GetOrigin();	
	double        tm = abs((pos.y) / ray.GetDirection().y);
	glm::dvec3    target = ray.PointAt(tm);
	return  target;
}


void Application::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	Application* app = GetWindow(window);
	app->m_xPos = xpos;
	app->m_yPos = ypos;

	// 右键键旋转
	if (app->m_bRightFlg)
	{
		glm::dvec2  offset = glm::dvec2(xpos, ypos) - app->m_rightPosition;
		app->m_rightPosition = glm::dvec2(xpos, ypos);
		// 左右上下旋转
		app->m_camera.RotateViewY(offset.x * 0.01f);
		app->m_camera.RotateViewX(offset.y * 0.01f);
		app->m_camera.Update();
	}

	// 左键平移
	if (app->m_bLeftFlg)
	{
		glm::dvec2    pos(xpos, ypos);		
		/**
		*   首先计算出来一个像素和当前场景的比例
		*/
		Ray   ray0 = app->m_camera.CreateRayFromScreen(pos.x, pos.y);
		Ray   ray1 = app->m_camera.CreateRayFromScreen(app->m_leftPosition.x, app->m_leftPosition.y);
		

		glm::dvec3  pos0 = app->CalcIntersectPoint(ray0);
		glm::dvec3  pos1 = app->CalcIntersectPoint(ray1);

		glm::dvec3  offset3 = pos1 - pos0;	

		app->m_leftPosition = pos;
	
		glm::dvec3  newEye = app->m_camera.GetEye() + offset3 * 0.3;
		glm::dvec3  newTgt = app->m_camera.GetTarget() + offset3 * 0.3;

		// 限制旋转相机高度  Y轴   -10~100 之间
		if (newEye.y > 100 || newEye.y < -10)
		{
			return;
		}
		app->m_camera.SetEye(newEye);
		app->m_camera.SetTarget(newTgt);		
		// std::cout << "Right" << glm::to_string(app->m_camera.GetRight()) << std::endl;
		app->m_camera.Update();
		
	}
}

void Application::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
	Application* Application = GetWindow(window);
	Application->m_winHeight = height;
	Application->m_winWidth  = width;

	if (height == 0) { height = 1;} // 防止除零
	
	float aspect = static_cast<float>(width) / static_cast<float>(height);

	Application->m_camera.Perspective(45.0f, aspect, 0.1f, 500.0f);
	Application->m_camera.SetViewSize(float(width), float(height));
}

// 窗口最小化
void Application::WindowIconifyCallback(GLFWwindow* window, int iconified)
{
	Application* application = GetWindow(window);
	application->m_isMinimized = (iconified == GLFW_TRUE);
	//std::cout << application->m_isMinimized << std::endl;
}

// 滚轮旋转
void Application::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Application* app = GetWindow(window);
	// ！！调整相机与目标点距离 m_camera-> m_radius
	double    persent = 1;

	if (yoffset > 0)
	{
		persent = 0.7;
	}
	else
	{
		persent = 1.3;
	}

	//app->m_camera.SetEye(app->m_camera.GetEye() * persent);		
	app->m_camera.ScaleCameraByPos(glm::vec3(0, 0, 0), persent); // 围绕原点缩放
	app->m_camera.Update();

}

// 获取 glfw 窗口的用户数据，转换成 Application类指针。
Application* Application::GetWindow(GLFWwindow* window)
{
	void* userdata = glfwGetWindowUserPointer(window);
	return reinterpret_cast<Application*>(userdata);
}


void Application::Initialize(int width, int height, const char* title)
{
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_pWindow    = glfwCreateWindow(width, height, title, NULL, NULL);
	m_winWidth   = width;
	m_winHeight  = height;

	if (!m_pWindow)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// 设置glfw回调函数
	glfwSetKeyCallback(m_pWindow, KeyCallback);
	glfwSetMouseButtonCallback(m_pWindow, MouseButtonCallback);
	glfwSetCursorPosCallback(m_pWindow, CursorPositionCallback);
	glfwSetWindowSizeCallback(m_pWindow, WindowSizeCallback);
	glfwSetScrollCallback(m_pWindow, ScrollCallback);
	glfwSetWindowIconifyCallback(m_pWindow, WindowIconifyCallback);

	glfwMakeContextCurrent(m_pWindow);

	// 设置用户数据，记录当前类指针
	glfwSetWindowUserPointer(m_pWindow, this);


	// 设置第相机	

	m_camera.SetEye(glm::dvec3(0, 0, 5));
	m_camera.SetTarget(glm::dvec3(0, 0, 0));
	m_camera.SetRight(glm::dvec3(1, 0, 0));
	m_camera.CalcDir();

	m_camera.SetViewSize(m_winWidth, m_winHeight);
	m_camera.Perspective(45.0f, float(width) / float(height), 0.1f, 500.0f);

	m_camera.Update();
	  

	gladLoadGL();
	glfwSwapInterval(1);     // 垂直同步打开1 关闭0	
	glEnable(GL_DEPTH_TEST); // 开启深度测试
}

void Application::Run()
{
	Startup();    // 1.渲染前准备工作
	double curTime = 0;  // 帧类用时
	int    framesNumber = 0;  // 

	while (!glfwWindowShouldClose(m_pWindow))
	{ 
		if (m_isMinimized) 
		{
			glfwWaitEvents(); // 窗口最小化时，暂停渲染
			continue;
		}
		
		double   elapseTime = (double)m_timeStamp.GetElapsedSecond();
		m_elapsedTime += elapseTime;               // 累计用时			
		curTime += m_timeStamp.GetElapsedSecond(); // 计算帧率
		if (curTime >= 1.0f)
		{
			m_franerNuber = framesNumber;			
			framesNumber = 0;
			curTime = 0;	
		}
		else
		{
			framesNumber++;
		}

		m_timeStamp.Update();

		glfwGetFramebufferSize(m_pWindow, &m_winWidth, &m_winHeight);

		glViewport(0, 0, m_winWidth, m_winHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.3f, 0.3f, 1.0f);

		Render(); // 2.开始渲染

		glfwSwapBuffers(m_pWindow);

	    glfwWaitEvents();    // 等待事件，没有事件低渲染方式	

		// glfwPollEvents();  // 打开垂直同步时输出60帧率 ，关闭垂直同步 充分渲染
		// 
		// std::cout << m_franerNuber << std::endl;
	}
	Shutdown();   // 3.结束渲染
}
#pragma endregion 

void Application::Startup()
{
}

void Application::Render()
{
}

void Application::Shutdown()
{
	
}


unsigned Application::CreateTexture(const char* fileName)
{
	unsigned  texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 翻转图像
	// stbi_set_flip_vertically_on_load(true);
	
	// 加载并生成纹理
	int width, height, nrChannels;
	unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		if (nrChannels == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("加载失败\n");
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

