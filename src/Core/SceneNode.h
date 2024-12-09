#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Camera.h"
/* ͨ�ýڵ� */
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

	// ���þֲ��任
	void SetLocalTransform(const glm::mat4& transform)
	{
		m_LocalTransform = transform;
	}

	// ��ȡ�ֲ��任
	const glm::mat4& GetLocalTransform() const
	{
		return m_LocalTransform;
	}

	// Ӧ��ƽ��
	void Translate(const glm::vec3& offset) {
		m_LocalTransform = glm::translate(m_LocalTransform, offset);
	}

	// Ӧ����ת������-�Ƕ���ʽ��
	void Rotate(float angle, const glm::vec3& axis) {
		m_LocalTransform = glm::rotate(m_LocalTransform, glm::radians(angle), axis);
	}

	// Ӧ������
	void Scale(const glm::vec3& scaleFactor) {
		m_LocalTransform = glm::scale(m_LocalTransform, scaleFactor);
	}

	// ����ӽڵ�
	void AddChild(SceneNode* child)
	{
		if (child)
		{
			child->m_ParentNode = this;
			m_ChildNodes.push_back(child);
		}
	}

	// �Ƴ��ӽڵ�
	void RemoveChild(SceneNode* child)
	{
		auto it = std::find(m_ChildNodes.begin(), m_ChildNodes.end(), child);
		if (it != m_ChildNodes.end())
		{
			m_ChildNodes.erase(it);
		}
	}

	// ��Ⱦ�ڵ���ӽڵ�
	virtual void Render(const Camera& camera)
	{
		glm::mat4 view            = camera.GetView();                       // ��ȡ���������ͼ��ͶӰ����
		glm::mat4 projection      = camera.GetProject();
		glm::mat4 globalTransform = projection * view * m_LocalTransform; 	// ����ȫ�ֱ任����
		Draw(globalTransform);                                              // �ڵ�ǰ�ڵ���ִ�о�����Ⱦ����
		for (SceneNode* child : m_ChildNodes)                             	// �ݹ���Ⱦ�ӽڵ�
		{
			child->Render(camera);
		}
	}

	// ��ȡ���ڵ�
	SceneNode* GetParent() const
	{
		return m_ParentNode;
	}

protected:

	// ����ʵ�־������Ⱦ�߼�
	virtual void Draw(const glm::mat4& globalTransform) {
		// ���಻���κ���Ⱦ
	}

private:
	glm::mat4 m_LocalTransform;                 // �ڵ�ľֲ��任
	SceneNode* m_ParentNode;                    // ���ڵ�ָ��
	std::vector<SceneNode*> m_ChildNodes;       // �ӽڵ��б�

};
