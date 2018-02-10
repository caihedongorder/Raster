#pragma once
#include "lopengl.h"

namespace OpenGL
{
    class Camera
    {
    public:
        Camera(){
            ViewMatrixDirty = true;
            ProjectionMatrixDirty = true;
        }
        ~Camera(){}

        void setProjection(float InFov,float InAr,float InNearPlane,float InFarPlane) {
            setFOV( InFov );
            setAr( InAr );
            setNearPlane( InNearPlane );
            setFarPlane( InFarPlane );
        }
        void setFOV(float InFov){ ProjectionMatrixDirty = true ; fov = InFov; }
        float getFOV(float InFov) const { return fov ; }
        void setAr(float InAr) { ProjectionMatrixDirty = true ; ar = InAr; }
        float getAr() const { return ar ; }
        void setNearPlane( float InNearPlane ) { ProjectionMatrixDirty = true ; nearPlane = InNearPlane; }
        float getNearPlane() const { return nearPlane ; }
        void setFarPlane( float InFarPlane ) { ProjectionMatrixDirty = true ; farPlane = InFarPlane ; }
        float getFarPlane() const { return farPlane ; }

        void setView(const glm::vec3& InEyeAt,const glm::vec3& InTargetPosition , const glm::vec3& InUpDir) { 
            setEyeAt( InEyeAt ) ;
            lookAt( InTargetPosition );
            setUpDir( InUpDir );
        }

        void move(const glm::vec3& InDelta){
            setEyeAt( eyeAt + InDelta );
        }
        void setEyeAt(const glm::vec3& InEyeAt) { ViewMatrixDirty = true ; eyeAt = InEyeAt ; }
        const glm::vec3& getEyeAt() const { return eyeAt ; }

        void rotate(float InRoll,float InPitch){
            glm::vec3 dir = lookDir;
            dir = glm::rotateX(dir,glm::radians(InRoll));
            dir = glm::rotateY(dir,glm::radians(InPitch));
            setLookDir(dir);
        }
        void lookAt(const glm::vec3& InTargetPosition ) { setLookDir( InTargetPosition - eyeAt ) ; }
        void setLookDir(const glm::vec3& InLookDir) { ViewMatrixDirty = true ; lookDir = InLookDir ; }
        const glm::vec3& getLookDir() const { return lookDir ; }

        void setUpDir(const glm::vec3& InUpDir) { ViewMatrixDirty = true ; upDir = InUpDir ; }
        const glm::vec3& getUpDir() const { return upDir ; }

        glm::vec3 getViewMatrixRight() { Update() ; return glm::vec3(ViewMatrix[0][0],ViewMatrix[1][0],ViewMatrix[2][0]);  }
        glm::vec3 getViewMatrixUp() { Update() ; return glm::vec3(ViewMatrix[0][1],ViewMatrix[1][1],ViewMatrix[2][1]);  }
        glm::vec3 getViewMatrixForward() { Update() ; return -glm::vec3(ViewMatrix[0][2],ViewMatrix[1][2],ViewMatrix[2][2]);  }

        const glm::mat4& getViewMatrix() { Update() ; return ViewMatrix; }
        const glm::mat4& getProjectionMatrix() { Update() ; return ProjectionMatrix; };
    private:
        void Update(){
            if(ViewMatrixDirty)
            {
                ViewMatrix = glm::lookAt(eyeAt,eyeAt + lookDir,upDir);
                ViewMatrixDirty = false;
            }
            if(ProjectionMatrixDirty)
            {
                ProjectionMatrix = glm::perspective(glm::radians(fov),ar,nearPlane,farPlane);
                ProjectionMatrixDirty = false;
            }
        }
    private:
        float fov;
        float ar;
        float nearPlane;
        float farPlane;

        glm::vec3 eyeAt;
        glm::vec3 lookDir;
        glm::vec3 upDir;

        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;

        bool ViewMatrixDirty;
        bool ProjectionMatrixDirty;
    };
}
