#pragma once
#include "DrawOpenGLSample.hpp"

namespace OpenGL
{
	class DrawMipmap : public RenderSample
	{
	private:
		struct Vertex {
			Raster::float3 position;
			Raster::float2 uv;
		};
	public:
		DrawMipmap(int w,int h){
			mWidth = w;
			mHeight = h;
		}
		void OnInit() {
			glClearColor(0, 0, 0, 0);

			//img = Raster::Image::LoadFromFile("images/tiger.png");
			Raster::Image imgBase = Raster::Image::BuidImage(256,256,Raster::RGBA(255,255,255,255));
			Raster::Image imglevel1 = Raster::Image::BuidImage(256,256,Raster::RGBA(255,0,0,255));
			Raster::Image imglevel2 = Raster::Image::BuidImage(128,128,Raster::RGBA(0,255,0,255));
			Raster::Image imglevel3 = Raster::Image::BuidImage(64,64,Raster::RGBA(0,0,255,255));
			Raster::Image imglevel4 = Raster::Image::BuidImage(32,32,Raster::RGBA(0,255,255,255));
			Raster::Image imglevel5 = Raster::Image::BuidImage(16,16,Raster::RGBA(255,0,255,255));
			Raster::Image imglevel6 = Raster::Image::BuidImage(8,8,Raster::RGBA(255,255,0,255));

			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &mtexture);
			glBindTexture(GL_TEXTURE_2D, mtexture);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imglevel1.GetWidth(), imglevel1.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imglevel1.getData());
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, imgBase.GetWidth(), imgBase.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, imgBase.getData());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imglevel1.GetWidth(), imglevel1.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imglevel1.getData());
			glTexImage2D(GL_TEXTURE_2D, 1, GL_RGBA, imglevel2.GetWidth(), imglevel2.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imglevel2.getData());
			glTexImage2D(GL_TEXTURE_2D, 2, GL_RGBA, imglevel3.GetWidth(), imglevel3.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imglevel3.getData());
			glTexImage2D(GL_TEXTURE_2D, 3, GL_RGBA, imglevel4.GetWidth(), imglevel4.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imglevel4.getData());
			glTexImage2D(GL_TEXTURE_2D, 4, GL_RGBA, imglevel5.GetWidth(), imglevel5.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imglevel5.getData());
			glTexImage2D(GL_TEXTURE_2D, 5, GL_RGBA, imglevel6.GetWidth(), imglevel6.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imglevel6.getData());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		void OnRender(float InDeltaTime){
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(60, double(mWidth) / double(mHeight), 0.1, 1000.0);
			float size = 5.0f;
			Vertex vertices[] = {
				{ Raster::float3(-size,0, size),Raster::float2(0,0)},
				{ Raster::float3(size,0, size),Raster::float2(1,0)},
				{ Raster::float3(size,0, -size),Raster::float2(1,1)},
				{ Raster::float3(-size,0, -size),Raster::float2(0,1)},
			};
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &vertices[0].position);
			glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertices[0].uv);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0, -1, -8);

			glEnable(GL_DEPTH_TEST);
			glBindTexture(GL_TEXTURE_2D, mtexture);

			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			//glDrawArrays(GL_QUADS,0,4); 
			glDrawArrays(GL_TRIANGLES,0,3); 
			glBindTexture(GL_TEXTURE_2D, 0);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	private:
		int mWidth;
		int mHeight;
		Raster::Image img;
		GLuint mtexture;
	};
}
