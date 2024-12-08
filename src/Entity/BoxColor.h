#pragma once
#include "../Entity/Entity.h"
#include "../Core/Camera.h"
#include <glm/gtc/type_ptr.hpp>

class BoxColor :public Entity
{
public:
    BoxColor(){};
	~BoxColor()
	{
        glDeleteVertexArrays(1, &m_cubeVAO);
        glDeleteVertexArrays(1, &m_lightVAO);
        glDeleteBuffers(1, &m_vbo);    
        glDeleteProgram(m_lightingShader);
        glDeleteProgram(m_cubeShader);	
	};
   
	virtual void Init()
	{
        const GLchar* lightingVS = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;
            
            void main()
            {
            	gl_Position = projection * view * model * vec4(aPos, 1.0);
            }
        )";
       
        const GLchar* lightingFS = R"(
            #version 330 core
            out vec4 FragColor;
              
            uniform vec3 objectColor;
            uniform vec3 lightColor;
            
            void main()
            {
                FragColor = vec4(lightColor * objectColor, 1.0);  // ！！！ 灯光 * 物体颜色
            }
         )";
     

        const GLchar* cubeVS = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;
            
            void main()
            {
            	gl_Position = projection * view * model * vec4(aPos, 1.0);
            }
        )";

        const GLchar* cubeFS = R"(
            #version 330 core
            out vec4 FragColor;
            
            void main()
            {
                FragColor = vec4(1.0); // set all 4 vector values to 1.0
            }
         )";


        // 创建 shader
        m_lightingShader = CreateShader(lightingVS, lightingFS);
        m_cubeShader     = CreateShader(cubeVS, cubeFS);


        float vertices[] = {
           -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
           -0.5f,  0.5f, -0.5f,
           -0.5f, -0.5f, -0.5f,
          
           -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
           -0.5f,  0.5f,  0.5f,
           -0.5f, -0.5f,  0.5f,
          
           -0.5f,  0.5f,  0.5f,
           -0.5f,  0.5f, -0.5f,
           -0.5f, -0.5f, -0.5f,
           -0.5f, -0.5f, -0.5f,
           -0.5f, -0.5f,  0.5f,
           -0.5f,  0.5f,  0.5f,
          
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
          
           -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
           -0.5f, -0.5f,  0.5f,
           -0.5f, -0.5f, -0.5f,
          
           -0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
           -0.5f,  0.5f,  0.5f,
           -0.5f,  0.5f, -0.5f,
        };

       
        glGenVertexArrays(1, &m_cubeVAO);
        glGenBuffers(1, &m_vbo);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(m_cubeVAO);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

       
        glGenVertexArrays(1, &m_lightVAO);
        glBindVertexArray(m_lightVAO);

        // we only need to bind to the m_vbo (to link it with glVertexAttribPointer), no need to fill it; the m_vbo's data already contains all we need (it's already bound, but we do it again for educational purposes)
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);


        m_lightingUniformProjection = glGetUniformLocation(m_lightingShader, "projection");
        m_lightingUniformView       = glGetUniformLocation(m_lightingShader, "view");
        m_lightingUniformModel      = glGetUniformLocation(m_lightingShader, "model");

        m_cubeUniformProjection     = glGetUniformLocation(m_cubeShader, "projection");
        m_cubeUniformView           = glGetUniformLocation(m_cubeShader, "view");
        m_cubeUniformModel          = glGetUniformLocation(m_cubeShader, "model");


        m_objectColor = glGetUniformLocation(m_lightingShader, "objectColor");
        m_lightColor = glGetUniformLocation(m_lightingShader, "lightColor");
	}

	virtual void  Render(const Camera &camera )
	{
        // lighting
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

        glm::mat4 projectionMat = camera.GetProject();
        glm::mat4 viewMat = camera.GetView();
        glm::mat4 modelMat = glm::mat4(1.0f);

        {   // 物体
            glUseProgram(m_lightingShader);



            glUniform3f(m_objectColor, 1.0f, 0.5f, 0.31f);
            glUniform3f(m_lightColor, 1.0f, 1.0f, 1.0f);


            glUniformMatrix4fv(m_lightingUniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMat));
            glUniformMatrix4fv(m_lightingUniformView, 1, GL_FALSE, glm::value_ptr(viewMat));
            glUniformMatrix4fv(m_lightingUniformModel, 1, GL_FALSE, glm::value_ptr(modelMat));

            glBindVertexArray(m_cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        {   // 灯光
           
            glUseProgram(m_cubeShader);

            modelMat = glm::mat4(1.0f);
            modelMat = glm::translate(modelMat, lightPos);
            modelMat = glm::scale(modelMat, glm::vec3(0.2f)); // a smaller cube


           
            glUniformMatrix4fv(m_cubeUniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMat));
            glUniformMatrix4fv(m_cubeUniformView, 1, GL_FALSE, glm::value_ptr(viewMat));
            glUniformMatrix4fv(m_cubeUniformModel, 1, GL_FALSE, glm::value_ptr(modelMat));

            glBindVertexArray(m_lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

	};


    unsigned CreateShader(const char* vertex, const char* fragment)
    {
        unsigned  shaderId;
        bool error = false;
        int  vertexShader = 0;
        int  fragmentShader = 0;

        do
        {
            if (vertex)
            {
                vertexShader = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(vertexShader, 1, &vertex, 0);
                glCompileShader(vertexShader);

                GLint   compileStatus;
                glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
                error = compileStatus == GL_FALSE;
                if (error)
                {
                    GLchar messages[256];
                    glGetShaderInfoLog(vertexShader, sizeof(messages), 0, messages);
                    printf("%s", messages);
                    assert(messages && 0 != 0);
                    break;
                }
            }

            if (fragment)
            {
                fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(fragmentShader, 1, &fragment, 0);
                glCompileShader(fragmentShader);

                GLint   compileStatus;
                glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
                error = compileStatus == GL_FALSE;

                if (error)
                {
                    GLchar messages[256];
                    glGetShaderInfoLog(fragmentShader, sizeof(messages), 0, messages);
                    printf("%s", messages);
                    assert(messages && 0 != 0);
                    break;
                }
            }
            shaderId = glCreateProgram();

            if (vertexShader)
            {
                glAttachShader(shaderId, vertexShader);
            }

            if (fragmentShader)
            {
                glAttachShader(shaderId, fragmentShader);
            }

            glLinkProgram(shaderId);

            GLint linkStatus;
            glGetProgramiv(shaderId, GL_LINK_STATUS, &linkStatus);
            if (linkStatus == GL_FALSE)
            {
                GLchar messages[256];
                glGetProgramInfoLog(shaderId, sizeof(messages), 0, messages);
                break;
            }
            glUseProgram(shaderId);

        } while (false);

        if (error)
        {
            if (fragmentShader)
            {
                glDeleteShader(fragmentShader);
                fragmentShader = 0;
            }
            if (vertexShader)
            {
                glDeleteShader(vertexShader);
                vertexShader = 0;
            }
            if (shaderId)
            {
                glDeleteProgram(shaderId);
                shaderId = 0;
            }
        }

        return shaderId;

    }

private:
    GLuint m_lightingShader = -1;
    GLuint m_cubeShader = -1;

    GLuint m_cubeVAO = -1;
    GLuint m_lightVAO = -1;
    GLuint m_vbo = -1;

    GLuint m_lightingUniformProjection = -1;
    GLuint m_lightingUniformView = -1;
    GLuint m_lightingUniformModel = -1;

    GLuint m_cubeUniformProjection = -1;
    GLuint m_cubeUniformView = -1;
    GLuint m_cubeUniformModel = -1;

    GLuint m_objectColor = -1;
    GLuint m_lightColor = -1;

};
