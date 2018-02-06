#pragma once
#include "DrawOpenGLSample.hpp"
#include "GLSLProgram.hpp"
#include "utils.h"

namespace OpenGL
{
	class GLSLDrawQuad : public RenderSample
	{

	private:
		GLuint mVBO;
		GLint mPositionLocation;
        GLSLProgram mProgram;
	private:
		struct Vertex
		{
			Raster::float4 position;
		};
	public:
		GLSLDrawQuad(){}
		virtual void OnInitShader(){
			Vertex pts[] = {
				{Raster::float4(-0.5f,0.5f,-5.0f,1.0f)},
				{Raster::float4(0.5f,0.5f,-5.0f,1.0f)},
				{Raster::float4(0.5f,-0.5f,-5.0f,1.0f)},
				{Raster::float4(-0.5f,-0.5f,-5.0f,1.0f)},
			};
			
			glGenBuffers(1, &mVBO);
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			
			mProgram.CreateProgram("shaders/test.vs","shaders/test.ps");

			mPositionLocation = glGetAttribLocation(mProgram.getProgram(), "vPosition");

			gluPerspective(60, float(800)/ float(600), 1, 1200);
								
		}
		void OnInit() {
			this->OnInitShader();
		}
		void OnRender(float InDeltaTime){
            glBindBuffer(GL_ARRAY_BUFFER, mVBO);
			mProgram.begin();
                glEnableVertexAttribArray(mPositionLocation);
                glVertexAttribPointer(mPositionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
                //glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);

                glDrawArrays(GL_QUADS, 0, 4);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
			mProgram.end();
            glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	};
}
