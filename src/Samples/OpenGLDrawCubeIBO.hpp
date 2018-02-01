#pragma once
#include "DrawOpenGLSample.hpp"
#include "image.hpp"
#include "Vector.hpp"


namespace OpenGL
{
	class DrawCubeIndicesIBO : public RenderSample
	{
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 color;
		};
	private:
		GLuint mVBO;
		GLuint mIBO;
		GLuint mIBOQuad;
	public:
		DrawCubeIndicesIBO(int width,int height){
			mWidth = width;
			mHeight = height;
		}
		void OnInit() {
			glEnable( GL_TEXTURE_2D );

			img = Raster::Image::LoadFromFile("images/tiger.png");
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.GetWidth(), img.GetHeight(), 0, GL_RGBA	, GL_UNSIGNED_BYTE, img.getData());
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);  
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);  
			glBindTexture(GL_TEXTURE_2D, 0);



			int l = - mWidth * 0.25;
			int t = mHeight * 0.25;
			int w = mWidth * 0.5;
			int h = mHeight * 0.5;

			Vertex vert[] = {
				// top
				{ glm::vec3( l , t ,w*0.5f )		,glm::vec3( 1 , 0 , 0 )	},
				{ glm::vec3( l + w , t ,w*0.5f )	,glm::vec3( 0 , 1 , 0 )	},
				{ glm::vec3( l + w , t ,-w*0.5f )	,glm::vec3( 0 , 0 , 1 )	},
				{ glm::vec3( l , t ,-w*0.5f )		,glm::vec3( 1 , 1 , 0 )	},

				//bottom
				{ glm::vec3( l , t-h ,w*0.5f )		,glm::vec3( 1 , 0 , 1 )	},
				{ glm::vec3( l , t-h ,-w*0.5f )		,glm::vec3( 0 , 1 , 1 )	},
				{ glm::vec3( l + w , t-h ,-w*0.5f )	,glm::vec3( 0 , 0 , 0 )	},
				{ glm::vec3( l + w , t-h ,w*0.5f )	,glm::vec3( 1 , 1 , 1 )	},
			};
			glGenBuffers(1, &mVBO);
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			GLuint indices[] = {
				// top bottom
				0,1,2,0,2,3, 
				4,5,6,4,6,7,

				//front back
				4,7,1,4,1,0,
				5,3,2,5,2,6,

				//left right
				7,6,2,7,2, 1,
				4,0,3,4,3, 5,
			};
			mIndexCountTriangles = sizeof(indices) / sizeof(indices[0]);
			glGenBuffers(1, &mIBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
			auto error = glGetError();

			GLuint indices_quad[] = {
				// top bottom
				0,1,2,3, 
				4,5,6,7,

				//front back
				4,7,1,0,
				5,3,2,6,

				//left right
				7,6,2,1,
				4,0,3,5,
			};
			mIndexCountQuads = sizeof(indices_quad) / sizeof(indices_quad[0]);
			glGenBuffers(1, &mIBOQuad);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBOQuad);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_quad), indices_quad, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
			glClearColor(0, 0, 0, 0);

#if 0
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, mWidth, mHeight, 0, 100, -100.0f);
#else

			glEnable(GL_CULL_FACE);
			
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(100, 0, 1000, 0, 0, 0, 0, 1, 0);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(60, mWidth*1.0f / mHeight, 1, 1200);
#endif
			RotationAngle = 0.0f;

			CamYMaxValue = 450;
			CamY = 300;
			CamChangeSpeed = -CamYMaxValue;


			//glFrontFace(GL_CW);
		}
		void OnRender(float InDeltaTime){
			RotationAngle += InDeltaTime * 60;
			CamY += CamChangeSpeed*InDeltaTime;
			if (Raster::Math::Abs(CamY) > CamYMaxValue)
			{
				CamChangeSpeed = -CamChangeSpeed;
				CamY = Raster::Math::Clamp(CamY, -CamYMaxValue, CamYMaxValue);
			}

			DrawRect(-mWidth*0.25f, mHeight*0.25f, mWidth*0.5f, mHeight*0.5f,mathfu::vec4(1,1,1,1));
			//glRasterPos2i(100,479);
			//glRasterPos2i(0,0);
			//glDrawPixels(img.GetWidth(), img.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, img.getData());
		}
	private:
		void DrawRect(int l, int t, int w, int h,const mathfu::vec4& color)
		{
			//glMatrixMode(GL_TEXTURE);
			//glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(0,CamY, 800, 0, 0, 0, 0, 1, 0);
			glRotatef(RotationAngle, 0, 1, 0);


			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//glBindTexture(GL_TEXTURE_2D, texture);
			glDisable(GL_TEXTURE_2D);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBOQuad);
			glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)0);
			glColorPointer(3, GL_FLOAT, sizeof(Vertex), (void*)12);

			//glDrawArrays(GL_QUADS,0,24);
			glDrawElements(GL_QUADS, mIndexCountQuads, GL_UNSIGNED_INT, nullptr);
			//glDrawElements(GL_TRIANGLES, mIndexCountTriangles, GL_UNSIGNED_INT, nullptr);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisable(GL_BLEND);
		}
	private:
		int mWidth, mHeight;
		int mIndexCountTriangles;
		int mIndexCountQuads;
		GLuint texture;
		Raster::Image img;
		float RotationAngle;
		float CamY;
		float CamYMaxValue;
		float CamChangeSpeed;
	};
}
