#pragma once
#include "LOpenGL.h"
#include "Math.hpp"

namespace OpenGL
{
    class PerlinNoise
    {
    public:
        enum{
            tableSize = 256,
            tableMask = tableSize -1,
        };
        PerlinNoise(){
            setup();
        }
        ~PerlinNoise(){

        }
        float noise(int x,int y,float scale){
            float xPos = x * scale;
            float yPos = y * scale;
            int xFloor = glm::floor( xPos );
            int yFloor = glm::floor( yPos );
            int xCeil = xFloor + 1 ;
            int yCeil = yFloor + 1 ;

            const glm::vec2& vec0 = getVec( xFloor , yFloor );
            const glm::vec2& vec1 = getVec( xCeil , yFloor );
            const glm::vec2& vec2 = getVec( xFloor , yCeil );
            const glm::vec2& vec3 = getVec( xCeil , yCeil );

            glm::vec2 d0 = glm::vec2(xPos - xFloor , yPos - yFloor );
            glm::vec2 d1 = glm::vec2(xPos - xCeil , yPos - yFloor );
            glm::vec2 d2 = glm::vec2(xPos - xFloor , yPos - yCeil );
            glm::vec2 d3 = glm::vec2(xPos - xCeil , yPos - yCeil );

            float h0 = glm::dot( vec0 , d0 );
            float h1 = glm::dot( vec1 , d1 );
            float h2 = glm::dot( vec2 , d2 );
            float h3 = glm::dot( vec3 , d3 );

            float Sx = 6*glm::pow(d0.x,5)
                -15* glm::pow(d0.x,4)
                +10.0f * glm::pow(d0.x,3);

            float Sy = 6*glm::pow(d0.y,5)
                -15* glm::pow(d0.y,4)
                +10.0f * glm::pow(d0.y,3);

            float t1 = Raster::Math::Lerp(h0,h2,Sy);
            float t2 = Raster::Math::Lerp(h1,h3,Sy);
            return Raster::Math::Lerp(t1,t2,Sx)*10.0f;
			/* return Raster::Math::Lerp(-1.0f, 1.0f, (Raster::Math::Lerp(t1, t2, Sx) + 1.0f)*0.5f)*100.0f; */
           /* return 1.0f; */
        }
    private:
        glm::vec2 mVecTable[tableSize];
        unsigned char mIndexTable[tableSize];

        void setup(){
            const float step = 6.28f/tableSize;
            float angle = 0;
            for(int i = 0 ; i < tableSize ; ++i )
            {
                mVecTable[i] = glm::vec2(cos(angle),sin(angle));
                angle += step;

                mIndexTable[i] = rand() & tableMask ;
            }
        }
        const glm::vec2& getVec(int x,int y) const{
            return mVecTable[ (mIndexTable [ x & tableMask ] + mIndexTable [ y & tableMask ]) & tableMask ];
        }

    };
}
