// 点光源
// 近处箱子被照亮 远处没有没被照亮。
#pragma once
#include "Entity.h"
#include "../Shader/ShaderCube.h"
#include "../Shader/ShaderLightingMaps.h"
#include "../Shader/ShaderLightingCastersPoint.h"

class BoxLightingCastersPoint :public Entity
{
public:
    BoxLightingCastersPoint() {}
	~BoxLightingCastersPoint()
	{
		glDeleteVertexArrays(1, &m_cubeVAO);
		glDeleteVertexArrays(1, &m_lightVAO);
        glDeleteTextures(1, &m_diffuseMap);
        glDeleteTextures(1, &m_specularMap);
		glDeleteBuffers(1,  &m_vbo);
        delete[] m_arrCubePositions;
	}

	void Init()
	{      
        m_shaderCube.Initialize(); // 初始化 shander
        m_shaderLightingCastersPoint.Initialize(); // 初始化 shander

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

       
        m_shaderLightingCastersPoint.SetInt("material.diffuse", 0);
        m_shaderLightingCastersPoint.SetInt("material.specular", 1);
             
       
        m_arrCubePositions = new glm::vec3[10]
        {
             glm::vec3(0.0f,  0.0f,  0.0f),
             glm::vec3(2.0f,  5.0f, -15.0f),
             glm::vec3(-1.5f, 2.2f, -2.5f),
             glm::vec3(-3.8f, 2.0f, -12.3f),
             glm::vec3(2.4f, -0.4f, -3.5f),
             glm::vec3(-1.7f, 3.0f, -7.5f),
             glm::vec3(1.3f,  3.5f, -2.5f),
             glm::vec3(1.5f,  2.0f, -2.5f),
             glm::vec3(1.5f,  0.2f, -1.5f),
             glm::vec3(-1.3f, 1.0f, -1.5f)
        };
	}


	virtual void Render(const Camera& camera)
	{      
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        glm::mat4 projectionMat = camera.GetProject();
        glm::mat4 viewMat = camera.GetView();
        glm::mat4 modelMat = glm::mat4(1.0f);

        //lightPos.x = sin(glfwGetTime()) * 2.0f;
        //lightPos.y = 0.8 + sin(glfwGetTime() / 2.0f) * 1.0f;

        //  绘制物体
        m_shaderLightingCastersPoint.Begin();
        {
            // be sure to activate shader when setting uniforms/drawing objects               
            m_shaderLightingCastersPoint.SetVec3("light.position", lightPos); // 定向光方向
            m_shaderLightingCastersPoint.SetVec3("viewPos", lightPos);

            // light properties
            m_shaderLightingCastersPoint.SetVec3("light.ambient",  0.2f, 0.2f, 0.2f);
            m_shaderLightingCastersPoint.SetVec3("light.diffuse",  0.5f, 0.5f, 0.5f);
            m_shaderLightingCastersPoint.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);
           
            m_shaderLightingCastersPoint.SetFloat("light.constant",  1.0f);
            m_shaderLightingCastersPoint.SetFloat("light.linear",    0.09f);
            m_shaderLightingCastersPoint.SetFloat("light.quadratic", 0.032f);
            // material properties
            m_shaderLightingCastersPoint.SetFloat("material.shininess", 1.0f);

            // view/projection transformations           
            m_shaderLightingCastersPoint.SetMat4("projection", projectionMat);
            m_shaderLightingCastersPoint.SetMat4("view", viewMat);
            m_shaderLightingCastersPoint.SetMat4("model", modelMat);

            // bind diffuse map
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_diffuseMap);
            // bind specular map
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_specularMap);

            
            // 绘制10个立方体
            glBindVertexArray(m_cubeVAO);
            for (GLuint i = 0; i < 10; i++)
            {                    
                modelMat = glm::mat4(1);
                modelMat = glm::translate(modelMat, m_arrCubePositions[i]);
                GLfloat angle = 20.0f * i;
                modelMat = glm::rotate(modelMat, angle, glm::vec3(1.0f, 0.3f, 0.5f));
                m_shaderLightingCastersPoint.SetMat4("model", modelMat);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            
        }
        m_shaderLightingCastersPoint.End();
       


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

    glm::vec3*         m_arrCubePositions =nullptr;

	ShaderCube         m_shaderCube;
    ShaderLightingCastersPoint m_shaderLightingCastersPoint;
public:
    GLuint m_diffuseMap = -1;  // 
    GLuint m_specularMap = -1; // 

};