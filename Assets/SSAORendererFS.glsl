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
int steps = 32;
float twoPi = 6.28318531;
float screenHeight = 1080.0f;
float screenWidth = 1920.0f;

// Variables determine how much occlusion affects the scene
float OCCLUSION_MODIFIER = 0.5;

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

vec2 rotateVec2(vec2 vector, float amount) {
    mat2 rotationMat = mat2(
        cos(amount), -sin(amount), sin(amount), cos(amount)
    );
    return rotationMat * vector;
}

void main() {
    if (renderMode == 1) {
        // Render just the depth to screen
        fragColour = vec4(vec3(linearize(texture(depthTexture, uv))), 1.0f);
    } else if (renderMode == 2) {
        // Do SSAO
        fragColour = vec4(texture(noiseTexture, screenToNoiseSample(uv)));
    } else if (renderMode == 3 || renderMode == 4) {
        float fragDepth = linearize(texture(depthTexture, uv));
        float occlusion = 0.0f;

        for (int i = 1; i < steps; i++) {
            // samplePos will be between -1 and 1
            vec2 samplePos = kernel[i];
            float rotationAmount = texture(noiseTexture, screenToNoiseSample(uv)).x * twoPi;

            // Then rotate by rotationAmount around origin
            samplePos = rotateVec2(samplePos, rotationAmount);

            // First scale by our depth
            //TODO - size 16 for now
            samplePos = samplePos * 16.0f;

            // Adjust for screen sample
            samplePos.x = samplePos.x / screenWidth;
            samplePos.y = samplePos.y / screenHeight;

            // Now sample
            float sampleDepth = linearize(texture(depthTexture, uv + samplePos));

            // If sample has less depth than fragDepth, add to occlusion
            if (fragDepth + bias < sampleDepth) {
                occlusion = occlusion + 1;
            }
        }

        occlusion = occlusion / 32.0f;
        occlusion = 1.0f - occlusion;

        if (renderMode == 3) {
            fragColour = vec4(vec3(occlusion), 1.0f);
        } else {
            vec3 colour = texture(sceneTexture, uv).xyz;
            vec3 finalColour = OCCLUSION_MODIFIER * occlusion * colour + (1 - OCCLUSION_MODIFIER) * colour;
            fragColour = vec4(finalColour, 1.0f);
        }
    } else {
        fragColour = texture(sceneTexture, uv);
    }
}
