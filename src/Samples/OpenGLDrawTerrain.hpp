#pragma once
#include "DrawOpenGLSample.hpp"
#include "image.hpp"
#include "Vector.hpp"
#include "Terrain.hpp"


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
            mTerrain.Init(glm::ivec3(256,25,256),glm::vec3(100,100,100));
		}
		void OnRender(float InDeltaTime){
            mTerrain.OnRender(InDeltaTime);
		}
	private:
        Terrain mTerrain;
        int mWidth,mHeight;
	};
}
