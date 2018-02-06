#pragma once
#include "DrawOpenGLSample.hpp"
#include "GLSLProgram.hpp"

namespace OpenGL
{
	class GLSLDrawQuad : public RenderSample,GLSLProgram
	{

	private:
		GLuint mVBO;
		GLint mPositionLocation;
	private:
		struct Vertex
		{
			Raster::float4 position;
		};
	public:
		GLSLDrawQuad(){}
		virtual void OnInitShader(){
			Vertex pts[] = {
				{Raster::float4(-0.5f,0.5f,0,1.0f)},
				{Raster::float4(0.5f,0.5f,0,1.0f)},
				{Raster::float4(0.5f,-0.5f,0,1.0f)},
				{Raster::float4(-0.5f,-0.5f,0,1.0f)},
			};
			
			glGenBuffers(1, &mVBO);
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			
			const char* vs = "attribute vec4 vPosition;"
				"void main()"
				"{"
					"gl_Position = vPosition;"
				"}";
			const char* ps = "void main()"
				"{"
					"gl_FragColor = vec4(1,0,0,1);"
				"}";
			CreateProgram(vs, ps);

			mPositionLocation = glGetAttribLocation(getProgram(), "vPosition");
								
		}
		void OnInit() {
			this->OnInitShader();
		}
		void OnRender(float InDeltaTime){
			this->begin();
                glEnableVertexAttribArray(mPositionLocation);
                glBindBuffer(GL_ARRAY_BUFFER, mVBO);
                //glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
                glVertexAttribPointer(mPositionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

                glDrawArrays(GL_QUADS, 0, 4);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
			this->end();
		}
	};
}
