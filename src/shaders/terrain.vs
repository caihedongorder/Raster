
attribute vec3 vPosition;
attribute float vHeight;
varying vec4 outColor;

uniform float heightScale;
uniform int IsDrawLine;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
    float height = vHeight ;
    vec4 posInView = V*M*vec4(vPosition.x , height * heightScale , vPosition.y , 1) ;
    if(IsDrawLine == 0)
        gl_Position = P * vec4(posInView.x,posInView.y+0.1,posInView.z,1.0);
    else
        gl_Position = P * posInView;

    /* height = 0.5; */
    outColor = vec4(0 , height , 0,1.0);
    /* gl_Position = gl_ModelViewProjectionMatrix * vec4(vPosition.x, 255.0 ,vPosition.y,1); */
};
