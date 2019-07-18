#include <graphics/ShaderProgram.h>

#include <Bedrock/PpException.h>

#include <vector>

namespace perfectpixel { namespace graphics {

ShaderProgram::ShaderProgram()
	: m_id(glCreateProgram())
	, m_linked(false)
{
}


ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::addShader(GLenum shaderType, const std::string &source)
{
	GLuint shader = glCreateShader(shaderType);

	const char *csource = source.c_str();

	glShaderSource(shader, 1, &csource, NULL);
	glCompileShader(shader);

	glAttachShader(m_id, shader);
}

void ShaderProgram::link()
{
	if (!m_linked)
	{
		glLinkProgram(m_id);
		GLint isLinked;

		glGetProgramiv(m_id, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint length;
			glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length);

			std::vector<GLchar> log(length);
			glGetProgramInfoLog(m_id, length, &length, &log[0]);


			throw bedrock::PpException("Failed to link program");
		}


		m_linked = true;
	}
}

void ShaderProgram::use()
{
	glUseProgram(m_id);
}

GLuint ShaderProgram::getUniformLocation(const std::string &name)
{
	return glGetUniformLocation(m_id, name.c_str());
}

}
}
