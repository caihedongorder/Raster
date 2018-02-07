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
			glm::vec2 position;
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
            mProgramDrawLineOrPoint.CreateProgram("shaders/terrain_line.vs","shaders/terrain_line.ps");
            mPositionLocation = glGetAttribLocation(mProgram.getProgram(),"vPosition");
            mColorLocation = glGetUniformLocation(mProgram.getProgram(),"vColor");
            mHeightLocation = glGetAttribLocation(mProgram.getProgram(),"vHeight");

            /* WorldSize = glm::vec3(4,1,4); */
			/* WorldScale = glm::vec3(3200, 1, 3200); */

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

            
            //创建abo
            glGenVertexArrays(1,&mABO);
            glBindVertexArray(mABO);

            //创建vbo
            std::vector<Vertex> verts; 
            for(int z = 0;z < VertexCountZ ; ++z)
            {
                currentX = startX;
                for(int x = 0 ; x < VertexCountX ; ++x)
                {
                    Vertex vert;
                    vert.position.x = currentX;
                    vert.position.y = currentZ;

                    verts.push_back(vert);

                    currentX +=StepX;
                }
                currentZ += StepY;
            }
            
            GLuint PositionVBO;
			glGenBuffers(1, &PositionVBO);
			glBindBuffer(GL_ARRAY_BUFFER, PositionVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*verts.size(), &verts[0], GL_STATIC_DRAW);
            glEnableVertexAttribArray(mPositionLocation);
            glVertexAttribPointer(mPositionLocation,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

            //创建高度 vbo
            std::vector<unsigned char> heightData;
            std::vector<unsigned char> heightDataProcessed;
            int HeightCountX = VertexCountX * SectionCount.x;
            int HeightCountY = VertexCountZ * SectionCount.y;
            int RawCountX = ( VertexCountX - 1 ) * SectionCount.x + 1;
            int RawCountY = ( VertexCountZ - 1 ) * SectionCount.y + 1;
            heightData.resize( RawCountX * RawCountY );
            memset(&heightData[0],0, RawCountX * RawCountY );
            TerrainUtil::evaluateHeightMidReplace(0,0, RawCountX - 1 , RawCountY - 1 , RawCountX ,128.0f,0.48f,&heightData[0]);

            heightDataProcessed.resize( HeightCountX * HeightCountY);
            memset(&heightDataProcessed[0],0, HeightCountX * HeightCountY );

            unsigned char* pDest = &heightDataProcessed[0];
            unsigned char* pSrc = & heightData[0];
            int DestStride = VertexCountX * VertexCountZ;
            int SrcStride = RawCountX ;
            for( int SectionY = 0; SectionY < SectionCount.y ; ++SectionY )
                for ( int SectionX = 0 ; SectionX < SectionCount.x ; ++SectionX )
                    for(int y = 0 ; y < VertexCountZ ; ++ y)
                        memcpy(pDest + ( SectionCount.x * SectionY + SectionX ) * DestStride + y * VertexCountX ,
                               pSrc + ( SectionY * ( VertexCountZ - 1 ) + y ) * SrcStride + SectionX * ( VertexCountX - 1 ) ,
                               VertexCountX);

            std::vector<float> fHeightDatas;
            fHeightDatas.resize(heightDataProcessed.size());
            for(int i = 0 ; i < fHeightDatas.size() ; ++ i)
            {
                fHeightDatas[i] = heightDataProcessed[i] / 255.0f;
            }

            glGenBuffers(1,&mHeightVBO);
            glBindBuffer(GL_ARRAY_BUFFER,mHeightVBO);
            /* glBufferData(GL_ARRAY_BUFFER, HeightCountX * HeightCountY , &heightDataProcessed[0] , GL_STATIC_DRAW); */
            glBufferData(GL_ARRAY_BUFFER, HeightCountX * HeightCountY * sizeof(float) , &fHeightDatas[0] , GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER,0);


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

            GLuint IBO;
			glGenBuffers(1, &IBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);

            glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
            glDisable(GL_BLEND);
			glClearColor(0, 0, 0, 0);
			
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(60, float(800)/ float(600), 1, 120000);

			RotationAngle = 0.0f;

			CamYMaxValue = 450;
			CamY = 15000;
			CamChangeSpeed = -CamYMaxValue;

        }

		void OnRender(float InDeltaTime){
			RotationAngle += InDeltaTime * 60;

            mProgram.begin();
                glUniform4f(mColorLocation,1,0,0,1);
                glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                /* glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); */
                OnRenderImpl();

                //绘制线
                /* glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); */
                /* glUniform4f(mColorLocation,0,0,1,1); */
                /* OnRenderImpl(0.1f); */



                //绘制点
                /* glPointSize(8.0f); */
                /* glUniform4f(mColorLocation,0,1,1,1); */
                /* glPolygonMode(GL_FRONT_AND_BACK,GL_POINT); */
                /* OnRenderImpl(0.1f); */
                /* glPointSize(1.0f); */

            mProgram.end();

		}
    private:
        void OnRenderImpl(float heightOffset = 0.0f){
            glBindVertexArray(mABO);
            glBindBuffer(GL_ARRAY_BUFFER,mHeightVBO);
            glEnableVertexAttribArray(mHeightLocation);
            for(int SectionY = 0 ; SectionY < SectionCount.y ; ++SectionY )
            {
                for(int SectionX = 0 ; SectionX < SectionCount.x ; ++SectionX )
                {
                    glMatrixMode(GL_MODELVIEW);
                    glLoadIdentity();
                    gluLookAt(0,CamY, 30000 , 0, 0, 0, 0, 1, 0);
                    glRotatef(RotationAngle, 0, 1, 0);
                    auto& Section =  TerrainSections [ SectionY * SectionCount.x + SectionX ];
                    glTranslatef(Section.SectionPosition.x,0 + heightOffset, Section.SectionPosition.y);

                    /* glVertexAttribPointer(mHeightLocation,1,GL_UNSIGNED_BYTE,GL_TRUE,0,(void*)( ( SectionY * SectionCount.x + SectionX) * SECTION_SIZE * SECTION_SIZE )); */
                    glVertexAttribPointer(mHeightLocation,1,GL_FLOAT,GL_FALSE,sizeof( float ),(void*)( ( SectionY * SectionCount.x + SectionX) * SECTION_SIZE * SECTION_SIZE * sizeof(float) ));

                    glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, nullptr);
                }
            }
            glDisableVertexAttribArray(mHeightLocation);
            glBindBuffer(GL_ARRAY_BUFFER,0);
            glBindVertexArray(0);
        }
    private:
        GLuint mABO;
        GLuint mHeightVBO;
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
        GLSLProgram mProgramDrawLineOrPoint;
        GLint mPositionLocation;
        GLint mColorLocation;
        GLint mHeightLocation;
    };
}
