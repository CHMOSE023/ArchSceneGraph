#include "MeshNode.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

MeshNode::MeshNode(const std::string& meshFilePath, Shader* shader) : m_Shader(shader), m_VAO(0), m_VBO(0), m_EBO(0), m_NumIndices(0) 
{
	LoadMesh(meshFilePath); // ������������
}

MeshNode::~MeshNode() { 
	if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
	if (m_VBO) glDeleteBuffers(1, &m_VBO);
	if (m_EBO) glDeleteBuffers(1, &m_EBO);
}

void MeshNode::LoadMesh(const std::string& meshFilePath) 
{
	GLfloat vertices[] = {
		// ��������           // ��ɫ
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // ���½� (��)
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // ���½� (��)
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // ���Ͻ� (��)
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // ���Ͻ� (��)
		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, // ���½� (Ʒ��)
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // ���½� (��)
		 0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.5f, // ���Ͻ� (��)
		-0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.5f  // ���Ͻ� (��)
	};

	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3,
		1, 5, 6,
		1, 6, 2,
		5, 4, 7,
		5, 7, 6,
		4, 0, 3,
		4, 3, 7,
		3, 2, 6,
		3, 6, 7,
		4, 5, 1,
		4, 1, 0
	};

	// ����VAO��VBO��EBO��������
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_NumIndices = sizeof(indices) / sizeof(indices[0]); 
}

void MeshNode::Render(const Camera& camera)
{
	m_Shader->Begin(); // ʹ����ɫ��
    glm::mat4 view            = camera.GetView();            	         // ��ȡ�������ͼ����
    glm::mat4 projection      = camera.GetProject();					 // ��ȡ�����ͶӰ����
	glm::mat4 globalTransform = projection * view * GetLocalTransform(); // ����ȫ�ֱ任����

	m_Shader->SetMat4("mvp", globalTransform); 	// ���� MVP ���� 

	Draw(globalTransform);   // ��Ⱦ����

	m_Shader->End();
}

void MeshNode::Draw(const glm::mat4& globalTransform) 
{
	glBindVertexArray(m_VAO); // ��VAO������
	glDrawElements(GL_TRIANGLES, m_NumIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
