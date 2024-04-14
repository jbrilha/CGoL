#version 410 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aOffset;
layout (location = 2) in int aState;

uniform vec3 color0;
uniform vec3 color1;
uniform vec3 color2;
uniform vec3 color3;

out vec3 color;

void main()
{
    gl_Position = vec4(aPos + aOffset, 0.f, 1.0);

    if(aState == 0) { color = color0; }
    else if(aState == 1 || aState == 3) { color = color1; }
    else if(aState == 2) { color = color2; }
    // else if(aState == 3) { color = color3; }
}
