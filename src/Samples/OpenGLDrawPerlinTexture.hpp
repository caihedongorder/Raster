#pragma once
#pragma once
#include "DrawOpenGLSample.hpp"
#include "image.hpp"
#include "Vector.hpp"
#include "TerrainUtil.hpp"
#include "PerlinNoise.hpp"

namespace OpenGL
{
	class DrawPerlinTexture : public RenderSample
	{
	public:
		DrawPerlinTexture(int width,int height){
			mWidth = width;
			mHeight = height;
		}
		void OnInit() {
            srand(time(nullptr));
			glEnable( GL_TEXTURE_2D );

			auto img = Raster::Image::LoadFromFile("images/tiger.png");

            std::vector<unsigned char> heightData;
            heightData.resize( 1025 * 1025 );
            /* TerrainUtil::evaluateHeightMidReplace(0,0,mWidth-1,mHeight-1,mWidth,128,0.5f,&heightData[0]); */
            TerrainUtil::evaluateHeightMidReplace(0,0,1025-1,1025-1,1025,128,0.5f,&heightData[0]); 

#if 1
            std::vector<unsigned char> imgData;
            imgData.resize(1025 * 1025 * 4);
            int nStride = 1025 * 4 ;
            for(int y = 0 ; y < 1025 ; ++y )
                for(int x = 0 ; x < 1025 ; ++x )
                {
                    unsigned char height =  heightData[ y * 1025 + x ];
                    imgData[y*nStride + x*4     ] = height;
                    imgData[y*nStride + x*4 + 1 ] = height;
                    imgData[y*nStride + x*4 + 2 ] = height;
                    imgData[y*nStride + x*4 + 3 ] = height;
                }
#else
            std::vector<unsigned char> imgData;
            imgData.resize(mWidth * mHeight * 4);
            int nStride = mWidth * 4 ;

            PerlinNoise noise;
            float Xscale = mWidth / PerlinNoise::tableSize;
            float Yscale = mHeight / PerlinNoise::tableSize;
            for(int y = 0 ; y < mHeight ; ++y )
                for(int x = 0 ; x < mWidth ; ++x )
                {
                    float value = glm::clamp(noise.noise(x,y,2.2f)+noise.noise(x,y,4.2f),0.0f,1.0f);
                    unsigned char height = value * 255;
                    imgData[y*nStride + x*4     ] = height;
                    imgData[y*nStride + x*4 + 1 ] = height;
                    imgData[y*nStride + x*4 + 2 ] = height;
                    imgData[y*nStride + x*4 + 3 ] = height;
                }

#endif


			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1025,1025, 0, GL_RGBA	, GL_UNSIGNED_BYTE, &imgData[0]);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth,mHeight, 0, GL_RGBA	, GL_UNSIGNED_BYTE, &imgData[0]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);  
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);  
			glBindTexture(GL_TEXTURE_2D, 0);




			glClearColor(0, 0, 0, 0);

			glMatrixMode(GL_PROJECTION);
			glOrtho(0, mWidth, mHeight, 0, -1, 1.0f);
		}
		void OnRender(float InDeltaTime){
			DrawRect(mWidth*0.25f, mHeight*0.25f, mWidth*0.5f, mHeight*0.5f,mathfu::vec4(1,1,1,1));
			//glRasterPos2i(100,479);
			//glRasterPos2i(0,0);
			//glDrawPixels(img.GetWidth(), img.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, img.getData());
		}
	private:
		void DrawRect(int l, int t, int w, int h,const mathfu::vec4& color)
		{
			//glMatrixMode(GL_TEXTURE);
			//glLoadIdentity();

			Raster::float2 pts[] = {
				Raster::float2(l,t),
				Raster::float2(l + w,t),
				Raster::float2(l + w,t + h),
				Raster::float2(l,t + h),
			};
			Raster::float2 texCoords[] = {
				Raster::float2(0,1.0f),
				Raster::float2(1.0f,1.0f),
				Raster::float2(1.0f,0.0f),
				Raster::float2(0.0f,0.0f),
			};

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glBindTexture(GL_TEXTURE_2D, texture);
#if 1
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 1.0f);
				glVertex2f(l, t);
				glTexCoord2f(1.0, 1.0f);
				glVertex2f(l+w, t);
				glTexCoord2f(1.0, 0.0f);
				glVertex2f(l+w, t+h);
				glTexCoord2f(0.0, 0.0f);
				glVertex2f(l, t+h);
			glEnd();
#else
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glVertexPointer(2, GL_FLOAT, 0, pts);
			glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
			glDrawArrays(GL_QUADS, 0, sizeof(pts) / sizeof(pts[0]));
#endif
			glDisable(GL_BLEND);


			//glDisableClientState(GL_VERTEX_ARRAY);
			//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			//glColor3f(color.x, color.y, color.z);
		}
		void DrawRectwithViewport(int l, int t, int w, int h,const mathfu::vec4& color)
		{
			glViewport(l, (mHeight - t) - h, w, h);
			glColor3f(color.x, color.y, color.z);
			glBegin(GL_QUADS);
				glVertex2f(0,0);
				glVertex2f(mWidth,0);
				glVertex2f(mWidth,mHeight);
				glVertex2f(0,mHeight);
			glEnd();
		}
	private:
		int mWidth, mHeight;
		GLuint texture;
	};
}
