#pragma once
#include "DrawOpenGLSample.hpp"
#include "image.hpp"
#include "Vector.hpp"
#include "Terrain.hpp"
#include "TerrainCLOD.hpp"


namespace OpenGL
{
	class DrawTerrain: public RenderSample
	{
	public:
		DrawTerrain(int width,int height){
			mWidth = width;
			mHeight = height;
		}
		void OnInit() {
            /* mTerrain.Init(glm::ivec3(1025,25,1025),glm::vec3(10000,10000,10000),mCam); */
            mTerrain.Init(glm::ivec3(65,25,65),glm::vec3(100,100,100),mCam);
            /* mTerrain.Init(glm::ivec3(256,25,256),glm::vec3(100,100,100),mCam); */
		}
		void OnRender(float InDeltaTime){
            mTerrain.OnRender(InDeltaTime);
		}
	private:
        /* Terrain mTerrain; */
        TerrainCLOD mTerrain;
        int mWidth,mHeight;
	};
}
