#pragma once
#include "../Entity/Entity.h"
#include "../Core/Camera.h"
#include "../Shader/ShaderCube.h"
#include "../Shader/ShaderMaterials.h"
#include <glm/gtc/type_ptr.hpp>

class BoxMaterials :public Entity
{  
public:
    BoxMaterials(){};
	~BoxMaterials()
	{
        glDeleteVertexArrays(1, &m_cubeVAO);
        glDeleteVertexArrays(1, &m_lightVAO);
        glDeleteBuffers(1, &m_VBO);
	};
   
	virtual void Init()
	{
     
        m_shaderCube.Initialize();
        m_shaderMaterials.Initialize();


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

        lightPos.x = sin(glfwGetTime()) * 2.0f;
        lightPos.y = 0.8 + sin(glfwGetTime() / 2.0f) * 1.0f;

        {  // 绘制物体 

            m_shaderMaterials.Begin();

            modelMat = glm::rotate(modelMat, (float)glfwGetTime(), glm::vec3(1, 1, 0));

            m_shaderMaterials.SetVec3("light.position", lightPos);         // 灯光位置
            m_shaderMaterials.SetVec3("viewPos",        camera.GetEye());  // 相机位置


            // light properties
            glm::vec3 lightColor;
            lightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0));
            lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7));
            lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3));
            glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);           // decrease the influence
            glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);         // low influence
            m_shaderMaterials.SetVec3("light.ambient",  ambientColor);
            m_shaderMaterials.SetVec3("light.diffuse",  diffuseColor);
            m_shaderMaterials.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

            // material properties
            m_shaderMaterials.SetVec3("material.ambient", 1.0f, 0.5f, 0.31f);
            m_shaderMaterials.SetVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
            m_shaderMaterials.SetVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
            m_shaderMaterials.SetFloat("material.shininess", 32.0f);


            // view/projection transformations
            glm::mat4 projection = camera.GetProject();
            glm::mat4 view = camera.GetView();
            m_shaderMaterials.SetMat4("projection", projection);
            m_shaderMaterials.SetMat4("view", view);

            // world transformation
            glm::mat4 model = glm::mat4(1.0f);
            m_shaderMaterials.SetMat4("model", model);

          
            glBindVertexArray(m_cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            m_shaderMaterials.End();

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
    ShaderCube      m_shaderCube;
    ShaderMaterials m_shaderMaterials;


};
