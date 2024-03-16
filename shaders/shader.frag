#version 410 core

out vec4 FragColor;

in vec3 color;

void main()
{
    FragColor = vec4(color, 1.f);
    // FragColor = vec4(1.f, 0.5f, 0.2f, 1.f);
}
