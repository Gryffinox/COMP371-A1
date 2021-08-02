#version 330 core

void main()
{
    // Don't need to explicitly tell the shader to do this because it's already automatically dont
    gl_FragDepth = gl_FragCoord.z;
}