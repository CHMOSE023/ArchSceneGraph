#pragma once
#include "tiny_gltf.h"
#include"../Core/SceneNode.h"
#include "../Shader/Shader.h" 
class Mesh : public SceneNode
{
public:
	Mesh(Shader* shader);

	virtual ~Mesh();

	virtual void Render(const Camera& camera) override; // 渲染网格节点

	void LoadMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh);      // 加载网格数据
protected:

	virtual void Draw(const glm::mat4& globalTransform) override; 	// 绘制网格
 
private:
	GLuint  m_VAO;         // 顶点数组对象	 
	Shader* m_Shader;      // 着色器对象
	int     m_NumIndices;  // 网格索引数量
	bool    m_DrawArrays = false;
	GLenum  m_IndexType;

};
