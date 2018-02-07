
attribute vec3 vPosition;
attribute float vHeight;
varying vec4 outColor;

uniform float heightScale;
uniform int IsDrawLine;

void main()
{
    float height = vHeight ;
    if(IsDrawLine == 0)
        gl_Position = gl_ModelViewProjectionMatrix * vec4(vPosition.x, height* heightScale ,vPosition.y,1);
    else
        gl_Position = gl_ModelViewProjectionMatrix * vec4(vPosition.x, height* heightScale + 0.1 ,vPosition.y,1);

    /* height = 0.5; */
    outColor = vec4(0 , height , 0,1.0);
    /* gl_Position = gl_ModelViewProjectionMatrix * vec4(vPosition.x, 255.0 ,vPosition.y,1); */
};
