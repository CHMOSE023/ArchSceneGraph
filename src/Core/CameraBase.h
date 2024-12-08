#pragma once
#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CameraBase
{
public:
	CameraBase();
	~CameraBase();
public:
	void UpdateCamera(double fElapsed, int key);
	void UpdateCamera(double fElapsed);


public:

	glm::dvec3  m_eye{ 0,0,0 };
	glm::dvec3  m_look{ 0,0,0 };
	glm::dvec3  m_up{ 0,0,0 };
	glm::dvec3  m_right{ 0,0,0 };

	glm::dmat4  m_view;

	double      m_moveSpeed;

};