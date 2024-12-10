#include "Mesh.h"

Mesh::Mesh(Shader* shader) : m_Shader(shader), m_VAO(0) , m_NumIndices(0)
{

} 

Mesh::~Mesh() 
{
	if (m_VAO) glDeleteVertexArrays(1, &m_VAO);	 
}
 

void Mesh::LoadMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh)
{
    // 1. 创建 VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // 准备存储 VBO IDs 的容器
    std::vector<GLuint> vbos; 

    const tinygltf::Primitive& primitive = mesh.primitives[0];

    // 2.1 顶点位置（POSITION）
    if (primitive.attributes.find("POSITION") != primitive.attributes.end())
    {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        vbos.push_back(vbo);

        const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.at("POSITION")];
        const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

        const GLfloat* data = reinterpret_cast<const GLfloat*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, accessor.count * 3 * sizeof(GLfloat), data, GL_STATIC_DRAW);

        // 设置顶点属性指针
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // 2.2 法线（NORMAL）
    if (primitive.attributes.find("NORMAL") != primitive.attributes.end())
    {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        vbos.push_back(vbo);

        const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.at("NORMAL")];
        const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

        const GLfloat* data = reinterpret_cast<const GLfloat*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, accessor.count * 3 * sizeof(GLfloat), data, GL_STATIC_DRAW);

        // 设置顶点属性指针
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // 2.3 纹理坐标（TEXCOORD_0）
    if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end())
    {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        vbos.push_back(vbo);

        const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
        const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

        const GLfloat* data = reinterpret_cast<const GLfloat*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, accessor.count * 2 * sizeof(GLfloat), data, GL_STATIC_DRAW);

        // 设置顶点属性指针
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // 3. 索引数据（绑定到 EBO）
    if (primitive.indices >= 0)
    {
        GLuint ebo;
        glGenBuffers(1, &ebo);

        // 绑定 EBO（索引缓冲对象只需要一个）
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
        const tinygltf::BufferView& indexBufferView = model.bufferViews[indexAccessor.bufferView];
        const tinygltf::Buffer& indexBuffer = model.buffers[indexBufferView.buffer];

        const GLuint* indexData = reinterpret_cast<const GLuint*>(&indexBuffer.data[indexBufferView.byteOffset + indexAccessor.byteOffset]);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexAccessor.count * sizeof(GLuint), indexData, GL_STATIC_DRAW);

        // 更新索引数量
        m_NumIndices = indexAccessor.count;
    }

    // 4. 解绑 VAO
    glBindVertexArray(0);

    // 释放临时存储的 VBO IDs（可以选择性地释放）
    for (GLuint vbo : vbos)
    {
        glDeleteBuffers(1, &vbo);
    }
    
}


void Mesh::Render(const Camera& camera)
{
	m_Shader->Begin(); // 使用着色器

	glm::mat4 view = camera.GetView();            	                     // 获取摄像机视图矩阵
	glm::mat4 projection = camera.GetProject();					         // 获取摄像机投影矩阵
	glm::mat4 globalTransform = projection * view * GetLocalTransform(); // 计算全局变换矩阵

	m_Shader->SetMat4("mvp", globalTransform); 	                         // 设置 MVP 矩阵 


	m_Shader->SetVec3("objectColor", 0.1f, 0.8f, 0.1f);  // 物体颜色
	m_Shader->SetVec3("lightColor", 1.0f, 1.0f, 1.0f);	 // 灯光颜色
	m_Shader->SetVec3("lightPos", camera.GetEye());   // 灯源位置
	m_Shader->SetVec3("viewPos", camera.GetEye());   // 相机位置

	m_Shader->SetMat4("projection", camera.GetProject());
	m_Shader->SetMat4("view", camera.GetView());
	m_Shader->SetMat4("model", glm::mat4(1.0f));

	Draw(globalTransform);   // 渲染网格

	m_Shader->End();
}

void Mesh::Draw(const glm::mat4& globalTransform)
{
	glBindVertexArray(m_VAO); // 绑定VAO并绘制

	if (m_DrawArrays)
	{
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, m_NumIndices, GL_UNSIGNED_INT, 0);
		 
	}

    glBindVertexArray(0);
}
