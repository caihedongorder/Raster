#pragma once
#include "LOpenGL.h"

namespace OpenGL
{
	class GLSLProgram
	{
	public:
		virtual void OnInitShader(){}

		void begin() {
			glUseProgram(mProgram);
		}
		void end() {
			glUseProgram(0);
		}
	protected:
		void CreateProgram(const char* vs, const char* ps) {
			auto vsHandle = glCreateShader(GL_VERTEX_SHADER);
			auto psHandle = glCreateShader(GL_FRAGMENT_SHADER);

			glShaderSource(vsHandle, 1, &vs, 0);

			glCompileShader(vsHandle);
			GLint status;
			glGetShaderiv(vsHandle, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE)
			{
				char compileError[MAXBYTE];
				glGetShaderInfoLog(vsHandle, MAXBYTE, 0, compileError);
				assert(false && compileError);
			}

			glShaderSource(psHandle, 1, &ps, 0);
			glCompileShader(psHandle);
			glGetShaderiv(psHandle, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE)
			{
				char compileError[MAXBYTE];
				glGetShaderInfoLog(psHandle, MAXBYTE, 0, compileError);
				assert(false && compileError);
			}

			mProgram = glCreateProgram();

			glAttachShader(mProgram, vsHandle);
			glAttachShader(mProgram, psHandle);

			glLinkProgram(mProgram);

			glGetShaderiv(mProgram, GL_LINK_STATUS, &status);
			if (status == GL_FALSE)
			{
				char LinkError[MAXBYTE];
				glGetShaderInfoLog(mProgram, MAXBYTE, 0, LinkError);
				assert(false && LinkError);
			}
		}
	private:
		GLuint mProgram;
	};
}
