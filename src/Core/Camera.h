#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Ray.h"
/// <summary>
/// 摄像机
/// </summary>
class Camera
{
public:	
	 Camera();	
	~Camera();
public:
	void        CalcDir();               // 计算观察方向
	glm::dvec3  GetEye() const;          
	void        SetEye(glm::dvec3 val);   // 设置眼睛的位置
	glm::dvec3  GetTarget() const;     
	void        SetTarget(glm::dvec3 val); // 设置目标点
	glm::dvec3  GetUp() const;
	void        SetUp(glm::dvec3 val) ;    // 设置向上方向
	glm::dvec3  GetDir() const;
	void        SetRight(glm::dvec3 val);
	glm::dvec3  GetRight() const;	
	void        SetViewSize(const glm::dvec4& viewSize);
	void        SetViewSize(double x, double y);
	glm::dvec2  GetViewSize();
	void        SetProject(const glm::dmat4& proj);
	const glm::dmat4& GetView()const;
	const glm::dmat4& GetProject()const;
	void        Ortho(double left, double right, double bottom, double top, double zNear, double zFar); // 正交投影
	void        Perspective(double fovy, double aspect, double zNear, double zFar);                     // 透视投影

	/**
	*   下面的函数的功能是将摄像机的观察方向绕某个方向轴旋转一定的角度改变观察者的位置，目标的位置不变化
	*/
	void       RotateViewX(double angle);
	void       RotateViewY(double angle);
	void       RotateViewXByCenter(double angle,glm::dvec3 pos);
	void       RotateViewYByCenter(double angle,glm::dvec3 pos);
	void       RotateViewZByCenter(double angle,glm::dvec3 pos);
	void       ScaleCameraByPos(const glm::dvec3& pos, double persent); // 指定点推进摄像机

	Ray        CreateRayFromScreen(int x, int y)const;                  // 产生一条射线  

	void       Update();                              

public:	
	
	glm::dvec3  m_eye;		// 眼睛位置
	glm::dvec3  m_up;		// 向上位置
	glm::dvec3  m_right;	// 朝右位置 
	glm::dvec3  m_target;	// 中心点
	glm::dvec3  m_dir;		// 观察方向  
	glm::dmat4  m_matView;	// 观察矩阵
	glm::dmat4  m_matProj;	// 投影矩阵（便于管理加入）
	glm::dmat4  m_matWorld;	// 世界矩阵（单位矩阵）

	glm::dvec4  m_viewSize;	// 视口大小（屏幕坐标与世界坐标之间转换时候用到）

	glm::dvec3  m_oldLength;	
};
