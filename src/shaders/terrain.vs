
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
    mat4 MVP = P*V*M;
    if(IsDrawLine == 0)
        gl_Position = MVP * vec4(vPosition.x, height* heightScale ,vPosition.y,1);
    else
        gl_Position = MVP * vec4(vPosition.x, height* heightScale + 0.1 ,vPosition.y,1);

    /* height = 0.5; */
    outColor = vec4(0 , height , 0,1.0);
    /* gl_Position = gl_ModelViewProjectionMatrix * vec4(vPosition.x, 255.0 ,vPosition.y,1); */
};
