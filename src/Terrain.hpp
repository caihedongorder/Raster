#pragma once
#include "lopengl.h"
#include "TerrainUtil.hpp"

namespace OpenGL
{
    struct TerrainSection{
        glm::vec2 SectionPosition;
    };
    class Terrain
    {
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 color;
		};
        enum{
            SECTION_SIZE = 32,
            SECTION_SHIFT = 5,
        };
    public:
        Terrain(){

        }
        ~Terrain(){

        }
        void Init(glm::ivec3 WorldSize , glm::vec3 WorldScale)
        {
            mProgram.CreateProgram("shaders/terrain.vs","shaders/terrain.ps");
            mPositionLocation = glGetAttribLocation(mProgram.getProgram(),"vPosition");

            std::vector<Vertex> verts; 

            SectionCount.x = (WorldSize.x + SECTION_SIZE - 1) >> SECTION_SHIFT;
            SectionCount.y = (WorldSize.z + SECTION_SIZE - 1) >> SECTION_SHIFT;
            SectionSize.x = (float)WorldSize.x / (float)SectionCount.x * WorldScale.x;
            SectionSize.y = (float)WorldSize.z / (float)SectionCount.y * WorldScale.z;

            float WorldSizeStartX = - WorldSize.x * 0.5f * WorldScale.x ;
            float WorldSizeStartY = - WorldSize.z * 0.5f * WorldScale.z ;
            glm::vec2 CurrentSectionPositon = glm::vec2( WorldSizeStartX , WorldSizeStartY );

            TerrainSections.resize( SectionCount.x * SectionCount.y );
            for( int SectionY = 0 ; SectionY < SectionCount.y ; ++ SectionY )
            {
                CurrentSectionPositon.x = WorldSizeStartX ;
                for(int SectionX = 0 ; SectionX < SectionCount.x ; ++ SectionX )
                {
                    auto& Section = TerrainSections[ SectionY * SectionCount.x + SectionX ];
                    Section.SectionPosition = CurrentSectionPositon;

                    CurrentSectionPositon.x += SectionSize.x ;
                }
                CurrentSectionPositon.y += SectionSize.y ;
            }

            int VertexCountX = SECTION_SIZE;
            int VertexCountZ = SECTION_SIZE;
            float StepX = SectionSize.x / (VertexCountX - 1) ;
            float StepY = SectionSize.y / (VertexCountZ - 1) ;
            float startX = 0;
            float startZ = 0;
            float currentX = startX;
            float currentZ = startZ;

            std::vector<unsigned char> heightData;
            heightData.resize(VertexCountX*VertexCountZ);
            memset(&heightData[0],0,VertexCountX*VertexCountZ);
            TerrainUtil::evaluateHeightMidReplace(0,0,VertexCountX-1,VertexCountZ-1,VertexCountX,128.0f,0.48f,&heightData[0]);
            //TerrainUtil::evaluateHeightMidReplace(int left,int top,int right,int bottom,int stride,float delta,float rough,unsigned char* HeightData)

            //创建vbo
            for(int z = 0;z < VertexCountZ ; ++z)
            {
                currentX = startX;
                for(int x = 0 ; x < VertexCountX ; ++x)
                {
                    Vertex vert;
                    vert.position.x = currentX;
                    vert.position.z = currentZ;
                    /* vert.position.y = heightData[z*VertexCountX+x]*0.1f; */
                    /* vert.position.y = heightData[z*VertexCountX+x]; */
                    vert.position.y = 0;

                    vert.color.x = vert.color.y = vert.color.z = 1.0f;

                    verts.push_back(vert);

                    currentX +=StepX;
                }
                currentZ += StepY;
            }

			glGenBuffers(1, &mVBO);
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*verts.size(), &verts[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);


            // 创建索引
            std::vector<GLuint> indices;
            currentX = 0;
            currentZ = 0;
            for(int z = 0 ; z < VertexCountZ - 1 ; ++z)
            {
                currentX = 0;
                for(int x = 0 ; x < VertexCountX - 1 ; ++ x)
                {
                    indices.push_back( ( currentZ +  1 )   * VertexCountX + currentX       );
                    indices.push_back(  currentZ            * VertexCountX + currentX + 1   );
                    indices.push_back(  currentZ            * VertexCountX + currentX       );

                    indices.push_back(( currentZ  +  1 )  * VertexCountX + currentX       );
                    indices.push_back(( currentZ  +  1 )  * VertexCountX + currentX + 1   );
                    indices.push_back(  currentZ            * VertexCountX + currentX + 1   );

                    currentX += 1;
                }
                currentZ += 1;
            }

			mIndexCount= indices.size();
			glGenBuffers(1, &mIBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
			glClearColor(0, 0, 0, 0);


			glEnable(GL_CULL_FACE);
			
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(100, 0, 4000, 0, 0, 0, 0, 1, 0);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(60, float(800)/ float(600), 1, 1200);

			RotationAngle = 0.0f;

			CamYMaxValue = 450;
			CamY = 500;
			CamChangeSpeed = -CamYMaxValue;

        }

		void OnRender(float InDeltaTime){
			RotationAngle += InDeltaTime * 60;

			glBindBuffer(GL_ARRAY_BUFFER, mVBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            mProgram.begin();
                glEnableVertexAttribArray(mPositionLocation);
                glVertexAttribPointer(mPositionLocation,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),0);
                OnRenderImpl();
            mProgram.end();
            glDisableVertexAttribArray(mPositionLocation);
            

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)0);
            //绘制线
            glColor3f(0,0,1);
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            OnRenderImpl(0.1f);

            //绘制点
            glColor3f(0,1,1);
            glPointSize(8.0f);
            glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
            OnRenderImpl(0.1f);
            glPointSize(1.0f);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisable(GL_BLEND);
		}
    private:
        void OnRenderImpl(float heightOffset = 0.0f){

            for(int SectionY = 0 ; SectionY < SectionCount.y ; ++SectionY )
            {
                for(int SectionX = 0 ; SectionX < SectionCount.x ; ++SectionX )
                {
                    glMatrixMode(GL_MODELVIEW);
                    glLoadIdentity();
                    gluLookAt(0,CamY, 600, 0, 0, 0, 0, 1, 0);
                    glRotatef(RotationAngle, 0, 1, 0);
                    auto& Section =  TerrainSections [ SectionY * SectionCount.x + SectionX ];
                    glTranslatef(Section.SectionPosition.x,0 + heightOffset, Section.SectionPosition.y);

                    glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, nullptr);
                }
            }


        }
    private:
		GLuint mVBO;
		GLuint mIBO;
		int mIndexCount;
		Raster::Image img;
		float RotationAngle;
		float CamY;
		float CamYMaxValue;
		float CamChangeSpeed;
        std::vector<TerrainSection> TerrainSections;
        glm::ivec2 SectionCount;
        glm::vec2 SectionSize;

        GLSLProgram mProgram;
        GLint mPositionLocation;
    };
}
