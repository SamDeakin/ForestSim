#version 330 core

in  vec2 uv;
out vec4 fragColour;
uniform sampler2D sceneTexture;

void main() {
    fragColour = texture(sceneTexture, uv.xy);
}
