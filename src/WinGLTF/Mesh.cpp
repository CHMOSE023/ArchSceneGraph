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
    // 1. ���� VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // ׼���洢 VBO IDs ������
    std::vector<GLuint> vbos; 

    const tinygltf::Primitive& primitive = mesh.primitives[0];

    // 2.0 ��ȡ����λ��
    if (primitive.indices >= 0)
    {
        GLuint ebo; // ��������������
        vbos.push_back(ebo);

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        const tinygltf::Accessor&   accessor   = model.accessors[primitive.indices];
        const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer&     buffer     = model.buffers[bufferView.buffer];

        const void* data = &buffer.data[bufferView.byteOffset + accessor.byteOffset];  // ������������

        // �����������ͽ��д���
        GLenum indexType = GL_UNSIGNED_SHORT; // Ĭ��ʹ�� UNSIGNED_SHORT

        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) 
        {
            indexType = GL_UNSIGNED_BYTE;
        }
        else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
        {
            indexType = GL_UNSIGNED_INT;
        }
     
        // �����������ݵ� EBO
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, accessor.count * tinygltf::GetComponentSizeInBytes(accessor.componentType), data, GL_STATIC_DRAW);
       
        // ����������������������
        m_NumIndices = accessor.count;
        m_IndexType  = indexType; // �����ֶ� m_IndexType ������������

    }

    // 2.1 ����λ�ã�POSITION��
    if (primitive.attributes.find("POSITION") != primitive.attributes.end())
    {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        vbos.push_back(vbo);
        auto indices = primitive.attributes.at("POSITION");
        const tinygltf::Accessor&    accessor   = model.accessors[indices];
        const tinygltf::BufferView&  bufferView = model.bufferViews[accessor.bufferView];
        const tinygltf::Buffer&      buffer     = model.buffers[bufferView.buffer];
        // buffer �� url
        const GLfloat* data = reinterpret_cast<const GLfloat*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, accessor.count * 3 * sizeof(GLfloat), data, GL_STATIC_DRAW);

        // ���ö�������ָ��
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0); 
    }

    // 2.2 ���ߣ�NORMAL��
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
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);   // ���ö�������ָ��
        glEnableVertexAttribArray(1);
    }

    // 4. ��� VAO
    glBindVertexArray(0);

    // �ͷ���ʱ�洢�� VBO IDs������ѡ���Ե��ͷţ�
    for (GLuint vbo : vbos)
    {
        glDeleteBuffers(1, &vbo);
    }
    
}


void Mesh::Render(const Camera& camera)
{
	m_Shader->Begin(); // ʹ����ɫ��

	glm::mat4 view = camera.GetView();            	                     // ��ȡ�������ͼ����
	glm::mat4 projection = camera.GetProject();					         // ��ȡ�����ͶӰ����
	glm::mat4 globalTransform = projection * view * GetLocalTransform(); // ����ȫ�ֱ任����

	m_Shader->SetMat4("mvp", globalTransform); 	                         // ���� MVP ���� 


	m_Shader->SetVec3("objectColor", 0.1f, 0.8f, 0.1f);  // ������ɫ
	m_Shader->SetVec3("lightColor", 1.0f, 1.0f, 1.0f);	 // �ƹ���ɫ
	m_Shader->SetVec3("lightPos", camera.GetEye());   // ��Դλ��
	m_Shader->SetVec3("viewPos", camera.GetEye());   // ���λ��

	m_Shader->SetMat4("projection", camera.GetProject());
	m_Shader->SetMat4("view", camera.GetView());
	m_Shader->SetMat4("model", glm::mat4(1.0f));

	Draw(globalTransform);   // ��Ⱦ����

	m_Shader->End();
}

void Mesh::Draw(const glm::mat4& globalTransform)
{
	glBindVertexArray(m_VAO); // ��VAO������

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
