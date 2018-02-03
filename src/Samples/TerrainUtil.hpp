#pragma once
#include "Math.hpp"

namespace OpenGL
{
    class TerrainUtil
    {
    public:
        static inline void evaluateHeightMidReplace(int left,int top,int right,int bottom,int stride,float delta,float rough,unsigned char* heightData)
        {
            float AH = heightData[top*stride+left];
            float BH = heightData[top*stride+right];
            float CH = heightData[bottom*stride+left];
            float DH = heightData[bottom*stride+right];
            if(AH == 0) heightData[top*stride+left]=AH=Raster::Math::RandInRange(128,255);
            if(BH == 0) heightData[top*stride+right]=BH=Raster::Math::RandInRange(128,255);
            if(CH == 0) heightData[bottom*stride+left]=CH=Raster::Math::RandInRange(128,255);
            if(DH == 0) heightData[bottom*stride+right]=DH=Raster::Math::RandInRange(128,255);


            int XMid = Raster::Math::Lerp(left,right,0.5f);
            int ZMid = Raster::Math::Lerp(top,bottom,0.5f);
           if(XMid > left) 
           {
                heightData[top*stride+XMid] = (AH + BH)*0.5f + Raster::Math::RandInRange(-delta*rough,delta*rough);
                heightData[bottom*stride+XMid] = (CH + DH)*0.5f + Raster::Math::RandInRange(-delta*rough,delta*rough);
           }
           if(ZMid > top)
           {
               float h1 = heightData[ZMid*stride+left] = (AH + CH) *0.5f + Raster::Math::RandInRange(-delta*rough,delta*rough);
               float h2 = heightData[ZMid*stride+right] = (BH + DH) *0.5f + Raster::Math::RandInRange(-delta*rough,delta*rough);
               if(XMid > left) 
               {
                   heightData[ZMid*stride+XMid] = (h1 + h2) *0.5f + Raster::Math::RandInRange(-delta*rough,delta*rough);
               }
           }

           if( XMid > left )
           {
               evaluateHeightMidReplace(left,top,XMid,ZMid,stride,delta*rough,rough,heightData);
               evaluateHeightMidReplace(XMid,top,right,ZMid,stride,delta*rough,rough,heightData);
               evaluateHeightMidReplace(left,ZMid,XMid,bottom,stride,delta*rough,rough,heightData);
               evaluateHeightMidReplace(XMid,ZMid,right,bottom,stride,delta*rough,rough,heightData);
           }

        }
    };
}
