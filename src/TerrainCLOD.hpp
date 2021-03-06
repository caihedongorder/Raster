#pragma once
#include "lopengl.h"
#include "TerrainUtil.hpp"
#include "Camera.hpp"

namespace OpenGL
{
    struct TerrainSectionCLOD{
        glm::mat4 ModelMatrix;
        int iLod = 1;
    };
    class TerrainCLOD
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
        TerrainCLOD(){
            mMaxLods = 2;
        }
        ~TerrainCLOD(){

        }
        void SetLOD(int iLod)
        {
            if( iLod >= mMaxLods ) return;
            
            for( auto & section : TerrainSections )
            {
                section.iLod = iLod;
            }
        }
        void Init(glm::ivec3 InWorldSize , glm::vec3 InWorldScale,Camera* InCam)
        {
            mCamera = InCam;

            mProgram.CreateProgram("shaders/terrain.vs","shaders/terrain.ps");
            mProgramDrawLineOrPoint.CreateProgram("shaders/terrain_line.vs","shaders/terrain_line.ps");
            mPositionLocation = glGetAttribLocation(mProgram.getProgram(),"vPosition");
            mColorLocation = glGetUniformLocation(mProgram.getProgram(),"vColor");
            mHeightLocation = glGetAttribLocation(mProgram.getProgram(),"vHeight");
            mIsDrawLineLocation = glGetUniformLocation(mProgram.getProgram(),"IsDrawLine");
            mHeightScaleLocation = glGetUniformLocation(mProgram.getProgram(),"heightScale");
            mModelMatrixLocation = glGetUniformLocation(mProgram.getProgram(),"M");
            mViewMatrixLocation = glGetUniformLocation(mProgram.getProgram(),"V");
            mProjectionMatrixLocation = glGetUniformLocation(mProgram.getProgram(),"P");

            WorldScale = InWorldScale;
            SectionCount.x = (InWorldSize.x - 1) >> SECTION_SHIFT;
            SectionCount.y = (InWorldSize.z - 1) >> SECTION_SHIFT;
            glm::vec2 SectionSize;
            SectionSize.x = 1.0f / SectionCount.x;
            SectionSize.y = 1.0f / SectionCount.y;

            //初始化terrainSection
            float WorldSizeStartX = - 0.5f ;
            float WorldSizeStartY = - 0.5f ;
            glm::vec2 CurrentSectionPositon = glm::vec2( WorldSizeStartX , WorldSizeStartY );

            TerrainSections.resize( SectionCount.x * SectionCount.y );
            glm::mat4 WorldScaleMat(1.0f);
            WorldScaleMat = glm::scale( WorldScaleMat , glm::vec3(InWorldScale.x , 1.0f , InWorldScale.z ));
            for( int SectionY = 0 ; SectionY < SectionCount.y ; ++ SectionY )
            {
                CurrentSectionPositon.x = WorldSizeStartX ;
                for(int SectionX = 0 ; SectionX < SectionCount.x ; ++ SectionX )
                {
                    auto& Section = TerrainSections[ SectionY * SectionCount.x + SectionX ];
                    glm::vec2 SectionPosition = CurrentSectionPositon;

                    Section.ModelMatrix = glm::mat4(1.0f);
                    Section.ModelMatrix = glm::translate(Section.ModelMatrix,glm::vec3( SectionPosition.x , 0 , SectionPosition.y ));

                    Section.ModelMatrix = WorldScaleMat * Section.ModelMatrix ;

                    CurrentSectionPositon.x += SectionSize.x ;
                }
                CurrentSectionPositon.y += SectionSize.y ;
            }

            //计算最大lod数
            int sz = SECTION_SIZE;
            mMaxLods = 1;
            while( sz > 1 )
            {
                ++mMaxLods;
                sz >>= 1;
            }

            //创建高度 vbo
            std::vector<unsigned char> heightData;
            std::vector<unsigned char> heightDataProcessed;
            int HeightCountX = InWorldSize.x;
            int HeightCountY = InWorldSize.z;
            heightData.resize( HeightCountX * HeightCountY );
            memset(&heightData[0],0, heightData.size() );
            TerrainUtil::evaluateHeightMidReplace(0,0, HeightCountX - 1 , HeightCountY - 1 , HeightCountX ,128.0f,0.48f,&heightData[0]);

            for( int iLod = 0 ; iLod < mMaxLods ; ++iLod )
            {
                int VertexCountX = (SECTION_SIZE >> iLod) + 1;
                int VertexCountZ = (SECTION_SIZE >> iLod) + 1;
                int heightDataProcessedCount = VertexCountX * VertexCountZ * SectionCount.x * SectionCount.y;
                heightDataProcessed.resize(heightDataProcessedCount);
                memset(&heightDataProcessed[0],0, heightDataProcessedCount );

                unsigned char* pDest = &heightDataProcessed[0];
                unsigned char* pSrc = &heightData[0];
                int DestStride = VertexCountX * VertexCountZ;
                int SrcStride = HeightCountX ;

                for( int SectionY = 0; SectionY < SectionCount.y ; ++SectionY )
                    for ( int SectionX = 0 ; SectionX < SectionCount.x ; ++SectionX )
                        for(int y = 0 ; y < VertexCountZ ; ++ y)
                            for(int x = 0 ; x < VertexCountX ; ++ x)
                                heightDataProcessed [ ( SectionY * SectionCount.x + SectionX ) * DestStride + y*VertexCountX + x ] = 
                                    heightData [ ( SectionY * SECTION_SIZE + ( y << iLod ) ) * SrcStride + ( SectionX * SECTION_SIZE ) + ( x << iLod )  ];
                                /* heightDataProcessed [ ( SectionY * SectionCount.x + SectionX ) * DestStride + y*VertexCountX + x ] = 128; */

                GLuint heightVBO;
                glGenBuffers(1,&heightVBO);
                glBindBuffer(GL_ARRAY_BUFFER,heightVBO);
                glBufferData(GL_ARRAY_BUFFER, heightDataProcessedCount , &heightDataProcessed[0] , GL_STATIC_DRAW);
                glBindBuffer(GL_ARRAY_BUFFER,0);

                mHeightVBOs.push_back( heightVBO ) ;

                //创建vbo
                float StepX = SectionSize.x / ( VertexCountX - 1 ) ;
                float StepY = SectionSize.y / ( VertexCountZ - 1 ) ;
                float currentX = 0;
                float currentZ = 0;
                std::vector<Vertex> verts; 
                for(int z = 0;z < VertexCountZ ; ++z)
                {
                    currentX = 0;
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
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                mPositionVBOs.push_back( PositionVBO ) ;


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

                mIndexCounts.push_back( indices.size() ) ;

                GLuint IBO;
                glGenBuffers(1, &IBO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

                mIBOs.push_back( IBO ) ;
            }


			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
            glDisable(GL_BLEND);
			glClearColor(0, 0, 0, 0);
			

			RotationAngle = 0.0f;

			CamYMaxValue = 450;
			CamY = 5000;
			CamChangeSpeed = -CamYMaxValue;

            mCamera->setProjection(60,800.0f/600.0f,1.0f,12000.0f);
            mCamera->setView(
                                      glm::vec3(0,CamY,3000),
                                      glm::vec3(0,0,0),
                                      glm::vec3(0,1,0)
                           );
        }

		void OnRender(float InDeltaTime){
			RotationAngle += InDeltaTime * 10;

            mProgram.begin();
                glUniformMatrix4fv(mViewMatrixLocation,1,GL_FALSE,glm::value_ptr(mCamera->getViewMatrix()));
                glUniformMatrix4fv(mProjectionMatrixLocation,1,GL_FALSE,glm::value_ptr(mCamera->getProjectionMatrix()));

                glUniform4f(mColorLocation,1,0,0,1);
                glUniform1f(mHeightScaleLocation,2550.0f*2);
                glUniform1i(mIsDrawLineLocation,0);
                glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                OnRenderImpl();
                

                glUniform1i(mIsDrawLineLocation,1);
                //绘制线
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                glUniform4f(mColorLocation,0,0,1,1);
                OnRenderImpl(0.1f);

                //绘制点
                glPointSize(8.0f);
                glUniform4f(mColorLocation,0,1,1,1);
                glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
                OnRenderImpl(0.1f);
                glPointSize(1.0f);

            mProgram.end();

		}
    private:
        void OnRenderImpl(float heightOffset = 0.0f){
            glEnableVertexAttribArray(mHeightLocation);
            glEnableVertexAttribArray(mPositionLocation);
            for(int SectionY = 0 ; SectionY < SectionCount.y ; ++SectionY )
            {
                for(int SectionX = 0 ; SectionX < SectionCount.x ; ++SectionX )
                {
                    auto& Section =  TerrainSections [ SectionY * SectionCount.x + SectionX ];

                    glUniformMatrix4fv(mModelMatrixLocation,1,GL_FALSE,glm::value_ptr(Section.ModelMatrix));

                    glBindBuffer(GL_ARRAY_BUFFER,mHeightVBOs[ Section.iLod ]);
                    glVertexAttribPointer(mHeightLocation,1,GL_UNSIGNED_BYTE,GL_TRUE,0,(void*)( ( SectionY * SectionCount.x + SectionX) * ( ( SECTION_SIZE >> Section.iLod ) + 1 ) * ( ( SECTION_SIZE >> Section.iLod ) + 1 )));


                    glBindBuffer(GL_ARRAY_BUFFER, mPositionVBOs[ Section.iLod ] );
                    glVertexAttribPointer(mPositionLocation,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),0);

                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBOs[ Section.iLod ]);
                    glDrawElements(GL_TRIANGLES, mIndexCounts[ Section.iLod ], GL_UNSIGNED_INT, nullptr);
                }
            }
            glDisableVertexAttribArray(mHeightLocation);
            glDisableVertexAttribArray(mPositionLocation);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        }
    private:
        std::vector<GLuint> mHeightVBOs;
        std::vector<GLuint> mPositionVBOs;
        std::vector<GLuint> mIBOs;
        std::vector<int> mIndexCounts;
		Raster::Image img;
		float RotationAngle;
		float CamY;
		float CamYMaxValue;
		float CamChangeSpeed;
        std::vector<TerrainSectionCLOD> TerrainSections;
        glm::ivec2 SectionCount;
        glm::vec3 WorldScale;
        int mMaxLods;


        GLSLProgram mProgram;
        GLSLProgram mProgramDrawLineOrPoint;
        GLint mPositionLocation;
        GLint mColorLocation;
        GLint mHeightLocation;
        GLint mIsDrawLineLocation;
        GLint mHeightScaleLocation;
        GLint mModelMatrixLocation;
        GLint mViewMatrixLocation;
        GLint mProjectionMatrixLocation;
        Camera * mCamera;
    };
}
