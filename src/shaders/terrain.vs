
attribute vec3 vPosition;
void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * vec4(vPosition,1);
};
