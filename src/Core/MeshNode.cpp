#include "MeshNode.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

MeshNode::MeshNode(const std::string& meshFilePath, Shader* shader) : m_Shader(shader), m_VAO(0), m_VBO(0), m_EBO(0), m_NumIndices(0) 
{
	LoadMesh(meshFilePath); // 加载网格数据
}

MeshNode::~MeshNode() { 
	if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
	if (m_VBO) glDeleteBuffers(1, &m_VBO);
	if (m_EBO) glDeleteBuffers(1, &m_EBO);
}

void MeshNode::LoadMesh(const std::string& meshFilePath) 
{
	GLfloat vertices[] = {
		// 顶点坐标           // 颜色
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 左下角 (红)
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 右下角 (绿)
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右上角 (蓝)
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // 左上角 (黄)
		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, // 左下角 (品红)
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // 右下角 (青)
		 0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.5f, // 右上角 (灰)
		-0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.5f  // 左上角 (灰)
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

	// 创建VAO、VBO、EBO并绑定数据
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
	m_Shader->Begin(); // 使用着色器
    glm::mat4 view            = camera.GetView();            	         // 获取摄像机视图矩阵
    glm::mat4 projection      = camera.GetProject();					 // 获取摄像机投影矩阵
	glm::mat4 globalTransform = projection * view * GetLocalTransform(); // 计算全局变换矩阵

	m_Shader->SetMat4("mvp", globalTransform); 	// 设置 MVP 矩阵 

	Draw(globalTransform);   // 渲染网格

	m_Shader->End();
}

void MeshNode::Draw(const glm::mat4& globalTransform) 
{
	glBindVertexArray(m_VAO); // 绑定VAO并绘制
	glDrawElements(GL_TRIANGLES, m_NumIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
