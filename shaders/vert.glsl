#version 410 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aOffset;
layout(location = 2) in int aState;

uniform vec3[10] colors;
uniform bool solid_colors;

out vec3 color;

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    gl_Position = vec4(aPos + aOffset, 0.f, 1.0);

    if (!solid_colors) {
        if (aState != 0) {
            color = mix(colors[aState] + vec3(rand(aOffset), rand(aOffset), rand(aOffset)) * 0.1f,
                    vec3(0.5 * rand(aOffset - 5), 0.3 * rand(aOffset + 5), 0.4 * rand(aPos)),
                    rand(aPos) * rand(aOffset)
                );
        } else {
            color = colors[aState] + vec3(rand(aOffset), rand(aOffset), rand(aOffset)) * 0.1f;
        }
    } else {
        color = colors[aState];
    }
}
