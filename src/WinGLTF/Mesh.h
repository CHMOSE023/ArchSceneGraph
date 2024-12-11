#pragma once
#include "tiny_gltf.h"
#include"../Core/SceneNode.h"
#include "../Shader/Shader.h" 
class Mesh : public SceneNode
{
public:
	Mesh(Shader* shader);

	virtual ~Mesh();

	virtual void Render(const Camera& camera) override; // ��Ⱦ����ڵ�

	void LoadMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh);      // ������������
protected:

	virtual void Draw(const glm::mat4& globalTransform) override; 	// ��������
 
private:
	GLuint  m_VAO;         // �����������	 
	Shader* m_Shader;      // ��ɫ������
	int     m_NumIndices;  // ������������
	bool    m_DrawArrays = false;
	GLenum  m_IndexType;

};
