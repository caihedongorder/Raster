
attribute vec4 vPosition;
void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * vPosition;
    /* gl_Position = vec4(vPosition.x,vPosition.y,0,1); */
};
