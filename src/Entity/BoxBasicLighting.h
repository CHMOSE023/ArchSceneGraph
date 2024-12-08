#pragma once
#include "../Entity/Entity.h"
#include "../Core/Camera.h"
#include "../Shader/ShaderBasicLighting.h"
#include "../Shader/ShaderCube.h"
#include <glm/gtc/type_ptr.hpp>

// 基础光照案例
class BoxBasicLighting :public Entity
{  
public:
    BoxBasicLighting(){};
	~BoxBasicLighting()
	{
        glDeleteVertexArrays(1, &m_cubeVAO);
        glDeleteVertexArrays(1, &m_lightVAO);
        glDeleteBuffers(1, &m_VBO);
	};
   
	virtual void Init()
	{     
        m_shaderCube.Initialize();
        m_shaderLighting.Initialize();


        float vertices[] = {
          -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
           0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
           0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
           0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
          -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
          -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
          
          -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
           0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
          -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
          -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
          
          -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
          -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
          -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
          -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
          -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
          -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
          
           0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
           0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
           0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
           0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
           0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
           0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
          
          -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
           0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
           0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
           0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
          -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
          -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
          
          -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
           0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
          -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
          -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };

       
        glGenVertexArrays(1, &m_cubeVAO);
        glGenBuffers(1, &m_VBO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // 盒子
        glBindVertexArray(m_cubeVAO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
       
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        
        // 光源
        glGenVertexArrays(1, &m_lightVAO);
        glBindVertexArray(m_lightVAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glVertexAttribPointer(m_shaderCube.m_position, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(m_shaderCube.m_position);

	}

	virtual void  Render(const Camera &camera )
	{  
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        glm::mat4 projectionMat = camera.GetProject();
        glm::mat4 viewMat = camera.GetView();
        glm::mat4 modelMat = glm::mat4(1.0f);

        // 灯光位置变化
        lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

        {  // 绘制物体 

            m_shaderLighting.Begin();

            modelMat = glm::rotate(modelMat, (float)glfwGetTime(), glm::vec3(1, 1, 0));

            m_shaderLighting.SetVec3("objectColor", 0.0f, 0.5f, 0.31f);
            m_shaderLighting.SetVec3("lightColor",  1.0f, 1.0f, 1.0f);
            m_shaderLighting.SetVec3("lightPos",    lightPos);
            m_shaderLighting.SetVec3("viewPos",     camera.GetEye());  // 相机位置

            m_shaderLighting.SetMat4("projection",  projectionMat);
            m_shaderLighting.SetMat4("view",        viewMat);
            m_shaderLighting.SetMat4("model",       modelMat);
          
            glBindVertexArray(m_cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            m_shaderLighting.End();

        }

        {  // 绘制光源 

            modelMat = glm::mat4(1.0f);
            modelMat = glm::translate(modelMat, lightPos);
            modelMat = glm::scale(modelMat, glm::vec3(0.2f)); // a smaller cube

            m_shaderCube.Begin();  
            {
                m_shaderCube.SetMat4("projection", projectionMat);
                m_shaderCube.SetMat4("view",       viewMat);
                m_shaderCube.SetMat4("model",      modelMat);

                glBindVertexArray(m_lightVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            m_shaderCube.End();
        }

	};

private:
    GLuint m_cubeVAO = -1;
    GLuint m_lightVAO = -1;
    GLuint m_VBO = -1;
    // 着色器
    ShaderCube           m_shaderCube ;
    ShaderBasicLighting  m_shaderLighting ;


};
