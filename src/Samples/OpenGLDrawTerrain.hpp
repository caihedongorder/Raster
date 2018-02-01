#pragma once
#pragma once
#include "DrawOpenGLSample.hpp"
#include "image.hpp"
#include "Vector.hpp"


namespace OpenGL
{
	class DrawTerrain : public RenderSample
	{
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 color;
		};
	private:
		GLuint mVBO;
		GLuint mIBOQuad;
	public:
		DrawTerrain(int width,int height){
			mWidth = width;
			mHeight = height;
		}
		void OnInit() {

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
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			
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


			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(0,CamY, 800, 0, 0, 0, 0, 1, 0);
            //
			//glFrontFace(GL_CW);
		}
		void OnRender(float InDeltaTime){
			//RotationAngle += InDeltaTime * 60;

			//glRotatef(RotationAngle, 0, 1, 0);



			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBOQuad);
			glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)0);
			glColorPointer(3, GL_FLOAT, sizeof(Vertex), (void*)12);

			glDrawElements(GL_QUADS, mIndexCountQuads, GL_UNSIGNED_INT, nullptr);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisable(GL_BLEND);
		}
	private:
		int mWidth, mHeight;
		int mIndexCountQuads;
		Raster::Image img;
		float RotationAngle;
		float CamY;
		float CamYMaxValue;
		float CamChangeSpeed;
	};
}
