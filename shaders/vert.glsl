#version 410 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aOffset;
layout (location = 2) in int aState;

uniform vec3[10] colors;

out vec3 color;

void main()
{
    gl_Position = vec4(aPos + aOffset, 0.f, 1.0);

    color = colors[aState];
}
