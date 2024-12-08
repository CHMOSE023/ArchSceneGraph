#include "Shader.h"

Shader::Shader():m_shaderId(0)
{
}

Shader::~Shader()
{
	// É¾³ýshader
	glDeleteProgram(m_shaderId);
}

bool Shader::CreateShader(const char* vertex, const char* fragment)
{
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
				assert(messages && 0 != 0);
				break;
			}
		}
		m_shaderId = glCreateProgram();

		if (vertexShader)
		{
			glAttachShader(m_shaderId, vertexShader);
		}

		if (fragmentShader)
		{
			glAttachShader(m_shaderId, fragmentShader);
		}

		glLinkProgram(m_shaderId);

		GLint linkStatus;
		glGetProgramiv(m_shaderId, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == GL_FALSE)
		{
			GLchar messages[256];
			glGetProgramInfoLog(m_shaderId, sizeof(messages), 0, messages);
			break;
		}
		glUseProgram(m_shaderId);

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
		if (m_shaderId)
		{
			glDeleteProgram(m_shaderId);
			m_shaderId = 0;
		}
	}

	return true;
}

void Shader::Begin()
{
	glUseProgram(m_shaderId);

}

void Shader::End()
{
	glUseProgram(0);

}

