#pragma once
#include <FreeImage.h>
#include <memory>

namespace Raster {
	class Image
	{
	public:
		Image(int InW, int InH, void* InData)
		{
			m_Width = InW;
			m_Height = InH;
			int dataSize = m_Width * m_Height * sizeof(unsigned int);
			m_Data = std::shared_ptr<unsigned int>(new unsigned int[m_Width*m_Height]);
#if 1
		unsigned int* DestPtr = (unsigned int*)m_Data.get();
		unsigned int* SrcPtr = (unsigned int*)InData;
#define GET_BYTE(inData,inByteIndex) (((inData)>>(8*inByteIndex))&0xff)
		for (int i = 0; i < m_Height*m_Width ; ++i)
		{
			DestPtr[i] = (GET_BYTE(SrcPtr[i],3)<<24) |
							(GET_BYTE(SrcPtr[i],0)<<16) |
							(GET_BYTE(SrcPtr[i],1)<<8) |
							GET_BYTE(SrcPtr[i],2);
		}
#else
		memcpy(m_Data.get(), InData, m_Width*m_Height * sizeof(unsigned int));
#endif
		}
		Image() {
			m_Width = m_Height = 0;
		}
		~Image() {
		}
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		inline RGBA PixelAt(int x, int y) {
			unsigned int* data = m_Data.get();
			return data[y*m_Width + x];
		}
	
		inline RGBA PixelFromUV(const float2 InUV) {
			return PixelAt(InUV.x*m_Width, InUV.y*m_Height);
		}
		inline bool IsValid() const {
			return m_Width != 0 && m_Height != 0 &&
				m_Data.get();
		}
		static Image LoadFromFile(const char* fileName)
		{
			Image img;
			//image format
			FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
			//pointer to the image, once loaded
			FIBITMAP *dib(0);
			//pointer to the image data
			BYTE* bits(0);
			//image width and height
			unsigned int width(0), height(0);
			
			//check the file signature and deduce its format
			fif = FreeImage_GetFileType(fileName, 0);
			//if still unknown, try to guess the file format from the file extension
			if(fif == FIF_UNKNOWN) 
				fif = FreeImage_GetFIFFromFilename(fileName);
			//if still unkown, return failure
			if(fif == FIF_UNKNOWN)
				return img;

			//check that the plugin has reading capabilities and load the file
			if(FreeImage_FIFSupportsReading(fif))
				dib = FreeImage_Load(fif, fileName);
			//if the image failed to load, return failure
			if(!dib)
				return img;

			dib = FreeImage_ConvertTo32Bits(dib);

			//retrieve the image data
			bits = FreeImage_GetBits(dib);
			//get the image width and height
			width = FreeImage_GetWidth(dib);
			height = FreeImage_GetHeight(dib);

			img = Image(width, height, bits);
			//Free FreeImage's copy of the data
			FreeImage_Unload(dib);
			return img;
		}
		const void* getData() const { return m_Data.get(); }
	private:
		int m_Width;
		int m_Height;
		std::shared_ptr<unsigned int> m_Data;
	};
}
