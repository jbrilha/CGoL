#version 410 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aOffset;

uniform mat4 model;
uniform vec3 aColor;

out vec3 color;

void main()
{
    gl_Position = model * vec4(aPos, 0.f, 1.0);

    color = aColor;
}
