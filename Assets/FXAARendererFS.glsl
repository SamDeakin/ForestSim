#version 330 core
// To be used with the basic TextureRendererVS.glsl

in  vec2 uv;
out vec4 fragColour;
uniform sampler2D sceneTexture;
uniform int renderMode;

void main() {
    if (renderMode == 0) {
        fragColour = texture(sceneTexture, uv.xy);
    }
}
