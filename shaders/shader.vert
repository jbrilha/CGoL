#version 410 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;
layout (location = 3) in int aState;

out vec3 color;

uniform mat4 model;
uniform vec3 uColor;

void main()
{
    gl_Position = vec4(aPos + aOffset, 0.f, 1.0);
    if(aState == 0) { color = vec3(0.2, 0.2, 0.2); }
    else { color = vec3(0.0, 1.0, 0.0); }
}
