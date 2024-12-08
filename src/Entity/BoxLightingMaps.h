#pragma once
#include "Entity.h"
#include "../Shader/ShaderCube.h"
#include "../Shader/ShaderLightingMaps.h"

class BoxLightingMaps :public Entity
{
public:
	BoxLightingMaps() {}
	~BoxLightingMaps() 
	{
		glDeleteVertexArrays(1, &m_cubeVAO);
		glDeleteVertexArrays(1, &m_lightVAO);
        glDeleteTextures(1, &m_diffuseMap);
        glDeleteTextures(1, &m_specularMap);
		glDeleteBuffers(1,  &m_vbo);
	}

	void Init()
	{
        m_shaderCube.Initialize();
		m_shaderLightingMaps.Initialize();

        float vertices[] = {
            // 顶点                   // 法线              // 纹理
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
        };

        glGenVertexArrays(1, &m_cubeVAO);
        glGenBuffers(1, &m_vbo);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(m_cubeVAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3,   GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2,       GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
        glGenVertexArrays(1, &m_lightVAO);
        glBindVertexArray(m_lightVAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        // note that we update the lamp's position attribute's stride to reflect the updated buffer data
        glVertexAttribPointer(m_shaderCube.m_position, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(m_shaderCube.m_position);

       
        m_shaderLightingMaps.SetInt("material.diffuse", 0);
        m_shaderLightingMaps.SetInt("material.specular", 1);

	}

	virtual void Render(const Camera& camera)
	{
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        glm::mat4 projectionMat = camera.GetProject();
        glm::mat4 viewMat = camera.GetView();
        glm::mat4 modelMat = glm::mat4(1.0f);

        lightPos.x = sin(glfwGetTime()) * 2.0f;
        lightPos.y = 0.8 + sin(glfwGetTime() / 2.0f) * 1.0f;

        { //  绘制物体
            m_shaderLightingMaps.Begin();
            {
                // be sure to activate shader when setting uniforms/drawing objects
                m_shaderLightingMaps.SetVec3("light.position", lightPos);
                m_shaderLightingMaps.SetVec3("viewPos", camera.GetEye());

                // light properties
                m_shaderLightingMaps.SetVec3("light.ambient", 0.2f, 0.2f, 0.2f);
                m_shaderLightingMaps.SetVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
                m_shaderLightingMaps.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

                // material properties
                m_shaderLightingMaps.SetFloat("material.shininess", 64.0f);

                // view/projection transformations
               
                m_shaderLightingMaps.SetMat4("projection", projectionMat);
                m_shaderLightingMaps.SetMat4("view", viewMat);

                // world transformation
                glm::mat4 model = glm::mat4(1.0f);
                m_shaderLightingMaps.SetMat4("model", model);

                // bind diffuse map
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_diffuseMap);
                // bind specular map
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, m_specularMap);
               
                // render the cube
                glBindVertexArray(m_cubeVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                
            }
            m_shaderLightingMaps.End();
        }

		
        {  // 绘制光源 

            modelMat = glm::mat4(1.0f);
            modelMat = glm::translate(modelMat, lightPos);
            modelMat = glm::scale(modelMat, glm::vec3(0.2f)); // a smaller cube

            m_shaderCube.Begin();
            {
                m_shaderCube.SetMat4("projection", projectionMat);
                m_shaderCube.SetMat4("view", viewMat);
                m_shaderCube.SetMat4("model", modelMat);

                glBindVertexArray(m_lightVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            m_shaderCube.End();
        }
	}

private:
	GLuint m_cubeVAO  = -1;
	GLuint m_lightVAO = -1;
	GLuint m_vbo      = -1;

	ShaderCube         m_shaderCube;
	ShaderLightingMaps m_shaderLightingMaps;
public:
    GLuint m_diffuseMap = -1;  // 
    GLuint m_specularMap = -1; // 

};