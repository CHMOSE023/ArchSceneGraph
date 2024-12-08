#include "Entity.h"
#include "../Shader/ShaderCube.h"
#include "../Shader/ShaderLightsMultiple.h"

class BoxLightsMultiple
{
public:
    BoxLightsMultiple() {};
    ~BoxLightsMultiple()
    {
        glDeleteVertexArrays(1, &m_cubeVAO);
        glDeleteVertexArrays(1, &m_lightVAO);
        glDeleteTextures(1, &m_diffuseMap);
        glDeleteTextures(1, &m_specularMap);
        glDeleteBuffers(1, &m_vbo);
        delete[] m_arrCubePositions;
    }

    void Init()
    {
        m_shaderCube.Initialize(); // 初始化 shander
        m_shaderLightsMultiple.Initialize(); // 初始化 shander

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
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
        glGenVertexArrays(1, &m_lightVAO);
        glBindVertexArray(m_lightVAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        // note that we update the lamp's position attribute's stride to reflect the updated buffer data
        glVertexAttribPointer(m_shaderCube.m_position, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(m_shaderCube.m_position);


        m_shaderLightsMultiple.SetInt("material.diffuse", 0);
        m_shaderLightsMultiple.SetInt("material.specular", 1);


        m_arrCubePositions = new glm::vec3[10]
        {
           glm::vec3(0.0f,  0.0f,  0.0f),
           glm::vec3(2.0f,  5.0f, -15.0f),
           glm::vec3(-1.5f, -2.2f, -2.5f),
           glm::vec3(-3.8f, -2.0f, -12.3f),
           glm::vec3(2.4f, -0.4f, -3.5f),
           glm::vec3(-1.7f,  3.0f, -7.5f),
           glm::vec3(1.3f, -2.0f, -2.5f),
           glm::vec3(1.5f,  2.0f, -2.5f),
           glm::vec3(1.5f,  0.2f, -1.5f),
           glm::vec3(-1.3f,  1.0f, -1.5f)
        };
    }


    virtual void Render(const Camera& camera)
    {
        glClearColor(0, 0, 0, 0);

        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        glm::mat4 projectionMat = camera.GetProject();
        glm::mat4 viewMat = camera.GetView();
        glm::mat4 modelMat = glm::mat4(1.0f);

        // 点光源位置
        static  glm::vec3 pointLightPositions[] =
        {
             glm::vec3(0.7f,  0.2f,  2.0f),
             glm::vec3(2.3f, -3.3f, -4.0f),
             glm::vec3(-4.0f,  2.0f, -12.0f),
             glm::vec3(0.0f,  0.0f, -3.0f)
        };

        //  绘制物体
        m_shaderLightsMultiple.Begin();
        {
            m_shaderLightsMultiple.SetVec3("viewPos", camera.GetEye());
            m_shaderLightsMultiple.SetFloat("material.shininess", 32.0f);

            // 在这里，我们为我们拥有的 5/6 种灯设置了所有制服。我们必须手动设置它们并编制索引
            // 数组中用于设置每个统一变量的正确 PointLight 结构。这可以更有利于代码
            // 通过将光源类型定义为类并在其中设置它们的值，或者使用更有效的统一方法
            // 通过使用“统一缓冲区对象”，但我们将在“高级 GLSL”教程中讨论。

            // directional light
            m_shaderLightsMultiple.SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            m_shaderLightsMultiple.SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
            m_shaderLightsMultiple.SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
            m_shaderLightsMultiple.SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
            // point light 1
            m_shaderLightsMultiple.SetVec3("pointLights[0].position", pointLightPositions[0]);
            m_shaderLightsMultiple.SetVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
            m_shaderLightsMultiple.SetVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
            m_shaderLightsMultiple.SetVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
            m_shaderLightsMultiple.SetFloat("pointLights[0].constant", 1.0f);
            m_shaderLightsMultiple.SetFloat("pointLights[0].linear", 0.09f);
            m_shaderLightsMultiple.SetFloat("pointLights[0].quadratic", 0.032f);
            // point light 2
            m_shaderLightsMultiple.SetVec3("pointLights[1].position", pointLightPositions[1]);
            m_shaderLightsMultiple.SetVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
            m_shaderLightsMultiple.SetVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
            m_shaderLightsMultiple.SetVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
            m_shaderLightsMultiple.SetFloat("pointLights[1].constant", 1.0f);
            m_shaderLightsMultiple.SetFloat("pointLights[1].linear", 0.09f);
            m_shaderLightsMultiple.SetFloat("pointLights[1].quadratic", 0.032f);
            // point light 3
            m_shaderLightsMultiple.SetVec3("pointLights[2].position", pointLightPositions[2]);
            m_shaderLightsMultiple.SetVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
            m_shaderLightsMultiple.SetVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
            m_shaderLightsMultiple.SetVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
            m_shaderLightsMultiple.SetFloat("pointLights[2].constant", 1.0f);
            m_shaderLightsMultiple.SetFloat("pointLights[2].linear", 0.09f);
            m_shaderLightsMultiple.SetFloat("pointLights[2].quadratic", 0.032f);
            // point light 4
            m_shaderLightsMultiple.SetVec3("pointLights[3].position", pointLightPositions[3]);
            m_shaderLightsMultiple.SetVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
            m_shaderLightsMultiple.SetVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
            m_shaderLightsMultiple.SetVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
            m_shaderLightsMultiple.SetFloat("pointLights[3].constant", 1.0f);
            m_shaderLightsMultiple.SetFloat("pointLights[3].linear", 0.09f);
            m_shaderLightsMultiple.SetFloat("pointLights[3].quadratic", 0.032f);
            // spotLight
            m_shaderLightsMultiple.SetVec3("spotLight.position", camera.GetEye());
            m_shaderLightsMultiple.SetVec3("spotLight.direction", camera.GetDir());
            m_shaderLightsMultiple.SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            m_shaderLightsMultiple.SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            m_shaderLightsMultiple.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            m_shaderLightsMultiple.SetFloat("spotLight.constant", 1.0f);
            m_shaderLightsMultiple.SetFloat("spotLight.linear", 0.09f);
            m_shaderLightsMultiple.SetFloat("spotLight.quadratic", 0.032f);
            m_shaderLightsMultiple.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            m_shaderLightsMultiple.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

            // view/projection transformations           
            m_shaderLightsMultiple.SetMat4("projection", projectionMat);
            m_shaderLightsMultiple.SetMat4("view", viewMat);
            m_shaderLightsMultiple.SetMat4("model", modelMat);

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
                m_shaderLightsMultiple.SetMat4("model", modelMat);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

        }
        m_shaderLightsMultiple.End();



        // 绘制光源位置

        m_shaderCube.Begin();

        m_shaderCube.SetMat4("projection", projectionMat);
        m_shaderCube.SetMat4("view", viewMat);

        for (size_t i = 0; i < 4; i++)
        {
            modelMat = glm::mat4(1.0f);
            modelMat = glm::translate(modelMat, pointLightPositions[i]);
            modelMat = glm::scale(modelMat, glm::vec3(0.2f)); // a smaller cube              
            m_shaderCube.SetMat4("model", modelMat);
            glBindVertexArray(m_lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        m_shaderCube.End();
    }

private:
    GLuint m_cubeVAO = -1;
    GLuint m_lightVAO = -1;
    GLuint m_vbo = -1;

    glm::vec3* m_arrCubePositions = nullptr;

    ShaderCube           m_shaderCube;
    ShaderLightsMultiple m_shaderLightsMultiple;
public:
    GLuint m_diffuseMap = -1;  // 
    GLuint m_specularMap = -1; // 

};

