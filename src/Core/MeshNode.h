#include "../Shader/Shader.h" 
#include"SceneNode.h"
/* 三维网格 */
class MeshNode : public SceneNode {
public:
	MeshNode(const std::string& meshFilePath, Shader* shader);
	virtual ~MeshNode();

	virtual void Render(const Camera& camera) override; // 渲染网格节点

protected:

	void LoadMesh(const std::string& meshFilePath); 	            // 加载网格数据
	virtual void Draw(const glm::mat4& globalTransform) override; 	// 绘制网格

private:
	GLuint  m_VAO;         // 顶点数组对象
	GLuint  m_VBO;         // 顶点缓冲区对象
	GLuint  m_EBO;         // 索引缓冲区对象
	Shader* m_Shader;      // 着色器对象
	int     m_NumIndices;  // 网格索引数量

};
