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

    // 2.0 读取索引位置
    if (primitive.indices >= 0)
    {
        GLuint ebo; // 索引缓冲区对象
        vbos.push_back(ebo);

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        const tinygltf::Accessor&   accessor   = model.accessors[primitive.indices];
        const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer&     buffer     = model.buffers[bufferView.buffer];

        const void* data = &buffer.data[bufferView.byteOffset + accessor.byteOffset];  // 访问索引数据

        // 根据索引类型进行处理
        GLenum indexType = GL_UNSIGNED_SHORT; // 默认使用 UNSIGNED_SHORT

        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) 
        {
            indexType = GL_UNSIGNED_BYTE;
        }
        else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
        {
            indexType = GL_UNSIGNED_INT;
        }
     
        // 加载索引数据到 EBO
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, accessor.count * tinygltf::GetComponentSizeInBytes(accessor.componentType), data, GL_STATIC_DRAW);
       
        // 保存索引数量和索引类型
        m_NumIndices = accessor.count;
        m_IndexType  = indexType; // 新增字段 m_IndexType 保存索引类型

    }

    // 2.1 顶点位置（POSITION）
    if (primitive.attributes.find("POSITION") != primitive.attributes.end())
    {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        vbos.push_back(vbo);
        auto indices = primitive.attributes.at("POSITION");
        const tinygltf::Accessor&    accessor   = model.accessors[indices];
        const tinygltf::BufferView&  bufferView = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer&      buffer     = model.buffers[bufferView.buffer];
        // buffer 是 url
        const GLfloat* data = reinterpret_cast<const GLfloat*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, accessor.count * 3 * sizeof(GLfloat), data, GL_STATIC_DRAW);

        // 设置顶点属性指针
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0); 
    }

    // 2.2 法线（NORMAL）
    if (primitive.attributes.find("NORMAL") != primitive.attributes.end())
    {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        vbos.push_back(vbo);
        auto indices = primitive.attributes.at("NORMAL");
        const tinygltf::Accessor&   accessor   = model.accessors[indices];
        const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer&     buffer     = model.buffers[bufferView.buffer];

        const GLfloat* data = reinterpret_cast<const GLfloat*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, accessor.count * 3 * sizeof(GLfloat), data, GL_STATIC_DRAW);  
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);   // 设置顶点属性指针
        glEnableVertexAttribArray(1);
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
		glDrawElements(GL_TRIANGLES, m_NumIndices, m_IndexType, nullptr);
	}

    glBindVertexArray(0);
}
