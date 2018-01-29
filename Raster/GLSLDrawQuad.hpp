#pragma once
#include "DrawOpenGLSample.hpp"
#include "GLSLProgram.hpp"

namespace OpenGL
{
	class GLSLDrawQuad : public RenderSample,GLSLProgram
	{
	private:
		struct Vertex
		{
			Raster::float3 position;
		};
	public:
		GLSLDrawQuad(){}
		virtual void OnInitShader(){
			const char* vs = "void main()"
				"{"
					"gl_Position = ftransform();"
				"}";
			const char* ps = "void main()"
				"{"
					"gl_FragColor = vec4(1,0,0,1);"
				"}";
			CreateProgram(vs, ps);
								
		}
		void OnInit() {
			this->OnInitShader();
		}
		void OnRender(float InDeltaTime){
			this->begin();
			Vertex pts[] = {
				{Raster::float3(-0.5f,0.5f,0)},
				{Raster::float3(0.5f,0.5f,0)},
				{Raster::float3(0.5f,-0.5f,0)},
				{Raster::float3(-0.5f,-0.5f,0)},
			};
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &pts[0].position);
			glDrawArrays(GL_QUADS, 0, 4);
			this->end();
		}
	};
}
