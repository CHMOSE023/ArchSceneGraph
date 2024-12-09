#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Camera.h"
/* 通用节点 */
class SceneNode 
{
public:
	SceneNode() : m_LocalTransform(glm::mat4(1.0f)), m_ParentNode(nullptr) {}

	virtual ~SceneNode()
	{
		for (SceneNode* child : m_ChildNodes) {
			delete child;
		}
		m_ChildNodes.clear();
	}

	// 设置局部变换
	void SetLocalTransform(const glm::mat4& transform)
	{
		m_LocalTransform = transform;
	}

	// 获取局部变换
	const glm::mat4& GetLocalTransform() const
	{
		return m_LocalTransform;
	}

	// 应用平移
	void Translate(const glm::vec3& offset) {
		m_LocalTransform = glm::translate(m_LocalTransform, offset);
	}

	// 应用旋转（以轴-角度形式）
	void Rotate(float angle, const glm::vec3& axis) {
		m_LocalTransform = glm::rotate(m_LocalTransform, glm::radians(angle), axis);
	}

	// 应用缩放
	void Scale(const glm::vec3& scaleFactor) {
		m_LocalTransform = glm::scale(m_LocalTransform, scaleFactor);
	}

	// 添加子节点
	void AddChild(SceneNode* child)
	{
		if (child)
		{
			child->m_ParentNode = this;
			m_ChildNodes.push_back(child);
		}
	}

	// 移除子节点
	void RemoveChild(SceneNode* child)
	{
		auto it = std::find(m_ChildNodes.begin(), m_ChildNodes.end(), child);
		if (it != m_ChildNodes.end())
		{
			m_ChildNodes.erase(it);
		}
	}

	// 渲染节点和子节点
	virtual void Render(const Camera& camera)
	{
		glm::mat4 view            = camera.GetView();                       // 获取摄像机的视图和投影矩阵
		glm::mat4 projection      = camera.GetProject();
		glm::mat4 globalTransform = projection * view * m_LocalTransform; 	// 计算全局变换矩阵
		Draw(globalTransform);                                              // 在当前节点上执行具体渲染操作
		for (SceneNode* child : m_ChildNodes)                             	// 递归渲染子节点
		{
			child->Render(camera);
		}
	}

	// 获取父节点
	SceneNode* GetParent() const
	{
		return m_ParentNode;
	}

protected:

	// 子类实现具体的渲染逻辑
	virtual void Draw(const glm::mat4& globalTransform) {
		// 基类不做任何渲染
	}

private:
	glm::mat4 m_LocalTransform;                 // 节点的局部变换
	SceneNode* m_ParentNode;                    // 父节点指针
	std::vector<SceneNode*> m_ChildNodes;       // 子节点列表

};
