
attribute vec3 vPosition;
attribute float vHeight;
varying vec4 outColor;
void main()
{
    float height = vHeight ;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(vPosition.x, height*10.0 ,vPosition.y,1);
    /* height = 0.5; */
    outColor = vec4(0 , height , 0,1.0);
    /* gl_Position = gl_ModelViewProjectionMatrix * vec4(vPosition.x, 255.0 ,vPosition.y,1); */
};
