#pragma once

#include <graphics/LocalGL.h>

#include <string>

namespace perfectpixel {
	namespace graphics {

		class ShaderProgram
		{
		public:
			ShaderProgram();
			~ShaderProgram();

		public:
			void addShader(GLenum shaderType, const std::string &source);
			void link();
			void use();

			GLuint getUniformLocation(const std::string &name);

		private:
			GLuint m_id;
			bool m_linked;
		};



	}
}