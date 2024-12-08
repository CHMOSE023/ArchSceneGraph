#include  "CameraBase.h"
#include  <Windows.h>
CameraBase::CameraBase()
{
	// vec3初始化
	m_eye   = glm::dvec3(0.0f);
	m_look  = glm::dvec3(0.0f);;
	m_up    = glm::dvec3(0.0f);;
	m_right = glm::dvec3(0.0f);;
	m_moveSpeed = 3.0f;
}

CameraBase::~CameraBase()
{
}

void CameraBase::UpdateCamera(double fElapsed, int key)
{
	glm::dvec3 tempLook = m_eye;
	glm::dvec3 dir      = m_look - m_eye;

	dir = glm::normalize(dir); // 归一化是指将向量的长度缩放为1，同时保持其方向不变

	if (265 == key) // 上键
	{
		m_eye -= (dir * - m_moveSpeed) * fElapsed;
		m_look -= (dir * - m_moveSpeed) * fElapsed;
	}

	if (264 == key) // 下键
	{
		m_eye += (dir * - m_moveSpeed) * fElapsed;
		m_look += (dir * - m_moveSpeed) * fElapsed;
	}

	if (262 == key) // 左键
	{
		m_eye -= (m_right * m_moveSpeed) * fElapsed;
		m_look -= (m_right * m_moveSpeed) * fElapsed;
	}

	if (263 == key) // 右键
	{
		m_eye += (m_right * m_moveSpeed) * fElapsed;
		m_look += (m_right * m_moveSpeed) * fElapsed;
	}


	m_view = glm::lookAt(m_eye, m_look, m_up);
}

void CameraBase::UpdateCamera(double fElapsed)
{
	glm::dvec3 tempLook  = m_eye;
	glm::dvec3 dir       = m_look - m_eye;

	dir = glm::normalize(dir); // 归一化是指将向量的长度缩放为1，同时保持其方向不变

	unsigned char keys[256];
	if (GetKeyboardState(keys)) 
	{

		if (keys[VK_UP] & 0x80)
		{
			m_eye  -= (dir * -m_moveSpeed) * fElapsed;
			m_look -= (dir * -m_moveSpeed) * fElapsed;
		}

		if (keys[VK_DOWN] & 0x80)
		{
			m_eye  += (dir * -m_moveSpeed) * fElapsed;
			m_look += (dir * -m_moveSpeed) * fElapsed;
		}

		if (keys[VK_LEFT] & 0x80)
		{
			m_eye  -= (m_right * m_moveSpeed) * fElapsed;
			m_look -= (m_right * m_moveSpeed) * fElapsed;
		}
		if (keys[VK_RIGHT] & 0x80)
		{
			m_eye  += (m_right * m_moveSpeed) * fElapsed;
			m_look += (m_right * m_moveSpeed) * fElapsed;
		}
	}

	
	m_view = glm::lookAt(m_eye,m_look,m_up);

}
