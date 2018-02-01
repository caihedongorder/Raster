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
            l = -1;
			int t = mHeight * 0.25;
            t = 0;
			int w = mWidth * 0.5;
            w = 2;

            std::vector<Vertex> verts; 

            int VertexCountX = 200;
            int VertexCountZ = 200;
            float Step = 0.5;
            float startX = -Step * VertexCountX * 0.5f;
            float startZ = -Step * VertexCountZ * 0.5f;
            float currentX = startX;
            float currentZ = startZ;

            for(int z = 0;z < VertexCountZ ; ++z)
            {
                currentX = startX;
                for(int x = 0 ; x < VertexCountX ; ++x)
                {
                    Vertex vert;
                    vert.position.x = currentX;
                    vert.position.z = currentZ;
                    vert.position.y = 0;

                    vert.color.x = vert.color.y = vert.color.z = 1.0f;

                    verts.push_back(vert);

                    currentX +=Step;
                }
                currentZ += Step;
            }

			glGenBuffers(1, &mVBO);
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*verts.size(), &verts[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);


            std::vector<GLuint> indices_quads;
            int currentQuadX = 0;
            int currentQuadZ = 0;
            for(int z = 0 ; z < VertexCountZ - 1 ; ++z)
            {
                currentQuadX = 0;
                for(int x = 0 ; x < VertexCountX - 1 ; ++ x)
                {
                    indices_quads.push_back(currentQuadZ        * VertexCountX + currentQuadX   );
                    indices_quads.push_back((currentQuadZ + 1 ) * VertexCountX + currentQuadX   );
                    indices_quads.push_back((currentQuadZ + 1 ) * VertexCountX + currentQuadX + 1);
                    indices_quads.push_back(currentQuadZ        * VertexCountX + currentQuadX + 1);

                    currentQuadX += 1;
                }
                currentQuadZ += 1;
            }

			mIndexCountQuads = indices_quads.size();
			glGenBuffers(1, &mIBOQuad);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBOQuad);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLuint)*indices_quads.size(), &indices_quads[0], GL_STATIC_DRAW);
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
			CamY = 10;
			CamChangeSpeed = -CamYMaxValue;


			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(0,CamY, 10, 0, 0, 0, 0, 1, 0);
            //glTranslatef(0,-300,0);
            //
			//glFrontFace(GL_CW);
		}
		void OnRender(float InDeltaTime){
			//RotationAngle += InDeltaTime * 60;

			//glRotatef(RotationAngle, 0, 1, 0);



			glEnableClientState(GL_VERTEX_ARRAY);
			//glEnableClientState(GL_COLOR_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBOQuad);
			glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)0);
			//glColorPointer(3, GL_FLOAT, sizeof(Vertex), (void*)12);

			glDrawElements(GL_QUADS, mIndexCountQuads, GL_UNSIGNED_INT, nullptr);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glDisableClientState(GL_VERTEX_ARRAY);
			//glDisableClientState(GL_COLOR_ARRAY);
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
