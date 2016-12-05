#version 330 core
// To be used with the basic TextureRendererVS.glsl

in  vec2 uv;
out vec4 fragColour;
uniform sampler2D sceneTexture;
uniform sampler2D depthTexture;
uniform int renderMode;
uniform sampler2D noiseTexture;
uniform vec2 kernel[32];

float far = 10000.0f;
float near = 0.5f;
float bias = 0.005f;

float linearize(vec4 depthInfo) {
    float depth = depthInfo.x * 2.0f - 1.0f;
    float linearized = 2.0f * near * far / (far + near - depth * (far - near));
    float final = (linearized - near) / far;
    return final;
}

vec2 screenToNoiseSample(vec2 coords) {
    float x = coords.x * 1920.0f / 32.0f;
    float y = coords.y * 1080.0f / 32.0f;
    return vec2(x, y);
}

void main() {
    if (renderMode == 1) {
        // Render just the depth to screen
        fragColour = vec4(vec3(linearize(texture(depthTexture, uv))), 1.0f);
    } else if (renderMode == 2) {
        // Do SSAO
        fragColour = vec4(texture(noiseTexture, screenToNoiseSample(uv)));
    } else {
        fragColour = texture(sceneTexture, uv);
    }
}
