#include "../Shader/Shader.h" 
#include"SceneNode.h"
/* ��ά���� */
class MeshNode : public SceneNode {
public:
	MeshNode(const std::string& meshFilePath, Shader* shader);
	virtual ~MeshNode();

	virtual void Render(const Camera& camera) override; // ��Ⱦ����ڵ�

protected:

	void LoadMesh(const std::string& meshFilePath); 	            // ������������
	virtual void Draw(const glm::mat4& globalTransform) override; 	// ��������

private:
	GLuint  m_VAO;         // �����������
	GLuint  m_VBO;         // ���㻺��������
	GLuint  m_EBO;         // ��������������
	Shader* m_Shader;      // ��ɫ������
	int     m_NumIndices;  // ������������

};
