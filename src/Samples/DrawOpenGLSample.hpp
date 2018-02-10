#pragma once
#include "LOpenGL.h"
#include "Camera.hpp"

namespace OpenGL
{
	class RenderSample 
	{
	public:
		RenderSample(){}
		virtual void OnInit(){}
		virtual void OnRender(float InDeltaTime){
		}
        void SetCamera(Camera* InCam)
        {
            mCam = InCam;
        }
        void CameraMove(const glm::vec3& InDelta){

        }
    protected:
        Camera* mCam;
	};
}
