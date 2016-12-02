#version 330 core

in vec3 position;
out vec2 uv;

void main() {
    // Simply sample the texture and return
    gl_Position = vec4(position, 1.0);
    uv = (position.xy + 1) / 2;
}
