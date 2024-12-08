#include"Ray.h"

Ray::Ray()
{
	m_origin = glm::vec3(0);
	m_direction = glm::vec3(0);
}

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
{
	m_origin = origin;
	m_direction = direction;
}

// 从屏幕空间产生一条射线类
void Ray::ScreenPointToRay(const glm::dvec2& screenPoint, const glm::dmat4& projectionMatrix, const glm::dmat4& viewMatrix, const glm::dvec4& viewport)
{
    // 从屏幕空间转换到裁剪空间
    double x = (2.0 * screenPoint.x - viewport.z) / viewport.z;
    double y = (2.0 * screenPoint.y - viewport.w) / viewport.w;
    glm::dvec4 rayClip(x, y, -1.0, 1.0);

    // 从裁剪空间转换到视图空间
    glm::dmat4 inverseProjectionMatrix = glm::inverse(projectionMatrix);
    glm::dvec4 rayView = inverseProjectionMatrix * rayClip;
    rayView = glm::dvec4(rayView.x, rayView.y, -1.0, 0.0); // 将方向向量的z分量设为负，因为视图空间中的z轴是朝向屏幕外的

    // 从视图空间转换到世界空间
    glm::dmat4 inverseViewMatrix = glm::inverse(viewMatrix);
    glm::dvec4 rayWorld = inverseViewMatrix * rayView;
    glm::dvec3 rayDirection = glm::normalize(glm::dvec3(rayWorld));

    m_origin    = glm::dvec3(0);
    m_direction = rayDirection;
    
}

//  设置射线的起点
void Ray::SetOrigin(const glm::vec3& origin)
{
	m_origin = origin;
}

// 设置射线的方向
void Ray::SetDirection(const glm::vec3& direction)
{

	m_direction = glm::normalize(direction);
}

// 获取射线的起点
glm::vec3 Ray::GetOrigin() const
{
	return m_origin;
}

// 获取射线的方向
glm::vec3 Ray::GetDirection() const
{
	return m_direction;
}

//  计算射线上的点
glm::vec3 Ray::PointAt(float t) const
{
	return m_origin + t * m_direction;

}
