#version 330 core
// To be used with the basic TextureRendererVS.glsl

in  vec2 uv;
out vec4 fragColour;
uniform sampler2D sceneTexture;
uniform int renderMode;

// These are hardcoded so don't try any other resolutions kids!
const float verticalOffset = 0.000926;
const float horizontalOffset = 0.000521;

// Tuning values
float EDGE_THRESHOLD = 0.2;
uint SEARCH_STEPS = 4u;

// Take a sample from a at offset distance from our pixel
vec3 getColourOffset(vec2 original, vec2 offset) {
    vec2 position = vec2(original.x + offset.x * horizontalOffset, original.y + offset.y * verticalOffset);
    return texture(sceneTexture, position).xyz;
}

// Get a luminance estimate from a colour
float luminance(vec3 rgb) {
    // We use a simple addition of these values
    return (rgb.r + rgb.g + rgb.b) / 3;

    // NVidia suggests a more complicated approach
    // This would give a weighted average of the r and g channels
    // They suggest ignoring the b channel because in practice it doesn't matter often
//    return rgb.g * (0.587 / 0.299) + rgb.r;
}

void main() {
    if (renderMode == 0) {
        // Render Mode 0 is just don't do FXAA
        fragColour = texture(sceneTexture, uv.xy);
    } else {
        // Here we actually do the FXAA
        // Start by assessing the greatest difference in dynamic range
        vec3 northColour = getColourOffset(uv, vec2(0, -1));
        vec3 westColour = getColourOffset(uv, vec2(-1, 0));
        vec3 centreColour = getColourOffset(uv, vec2(0, 0));
        vec3 eastColour = getColourOffset(uv, vec2(1, 0));
        vec3 southColour = getColourOffset(uv, vec2(0, 1));
        float northLum = luminance(northColour);
        float westLum = luminance(westColour);
        float centreLum = luminance(centreColour);
        float eastLum = luminance(eastColour);
        float southLum = luminance(southColour);

        float maxLum = max(max(max(northLum, westLum), max(eastLum, southLum)), centreLum);
        float minLum = min(min(min(northLum, westLum), min(eastLum, southLum)), centreLum);
        float range = maxLum - minLum;

        if (range < maxLum * EDGE_THRESHOLD) {
            fragColour = vec4(centreColour, 1.0);
            return;
        }

        if (renderMode == 1) {
            // Render Mode 1 just colours all areas we perform aliasing on red
            fragColour = vec4(1.0, 0.0, 0.0, 1.0);
            return;
        }

        // Test 4 corners
        vec3 NWColour = getColourOffset(uv, vec2(-1, -1));
        vec3 SEColour = getColourOffset(uv, vec2(1, 1));
        vec3 NEColour = getColourOffset(uv, vec2(1, -1));
        vec3 SWColour = getColourOffset(uv, vec2(-1, 1));
        float NWLum = luminance(NWColour);
        float SELum = luminance(SEColour);
        float NELum = luminance(NEColour);
        float SWLum = luminance(SWColour);

        // Add subpixel aliasing test
        // TODO

        // Test if edge is verticle or horizontal
        // We basically just take a weighted sum these all to figure out which direction we are closer to
        float vertical =
            abs((0.25 * NWLum) + (-0.5 * northLum) + (0.25 * NELum)) +
            abs((0.50 * westLum) + (-1.0 * centreLum) + (0.5 * eastLum)) +
            abs((0.25 * SWLum) + (-0.5 * southLum) + (0.25 * SELum));
        float horizontal =
            abs((0.25 * NWLum) + (-0.5 * westLum) + (0.25 * SWLum)) +
            abs((0.50 * northLum) + (-1.0 * centreLum) + (0.5 * southLum)) +
            abs((0.25 * NELum) + (-0.5 * eastLum) + (0.25 * SELum));

        if (renderMode == 2) {
            // Render Mode 2 shows which edges are which orientation
            if (vertical > horizontal) {
                fragColour = vec4(1.0, 0.0, 1.0, 1.0);
            } else {
                fragColour = vec4(1.0, 1.0, 0.0, 1.0);
            }
            return;
        }

        // Find end of edge
    }
}
