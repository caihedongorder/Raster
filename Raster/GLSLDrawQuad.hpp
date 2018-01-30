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
			glBindBuffer(GL_VERTEX_ARRAY, mVBO);
			int size = sizeof(pts);
			glBufferData(GL_VERTEX_ARRAY, size, pts, GL_STATIC_DRAW);
			glBindBuffer(GL_VERTEX_ARRAY, 0);
			
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
#if 0
			glBindBuffer(GL_VERTEX_ARRAY, mVBO);
			//glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
			glEnableVertexAttribArray(mPositionLocation);
			glVertexAttribPointer(mPositionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

			glDrawArrays(GL_QUADS, 0, 4);
			glBindBuffer(GL_VERTEX_ARRAY, 0);
#else
			Vertex pts[] = {
				{Raster::float4(-0.5f,0.5f,0,1.0f)},
				{Raster::float4(0.5f,0.5f,0,1.0f)},
				{Raster::float4(0.5f,-0.5f,0,1.0f)},
				{Raster::float4(-0.5f,-0.5f,0,1.0f)},
			};
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(4, GL_FLOAT, sizeof(Vertex), pts);
			glDrawArrays(GL_QUADS, 0, 4);
#endif
			this->end();
		}
	};
}
