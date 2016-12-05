#version 330

in vec3 vertexNormal;
in vec3 vertexPosition;
in vec4 shadowPosition;
in vec3 vertexKd;
in vec3 vertexKs;
in float vertexNs;

out vec4 fragColour;

// Light uniforms
uniform vec3 lightDirection;
uniform vec3 lightColour;
uniform float ambientIntensity;

uniform sampler2DShadow shadow;

const float SHADOW_SAMPLE_BIAS = 0.005;

vec3 phongModel(vec3 fragPosition, vec3 fragNormal) {
    // Direction from fragment to light source.
    vec3 l = normalize(-lightDirection);

    // Direction from fragment to viewer (origin - fragPosition).
    vec3 v = normalize(-fragPosition);

    vec3 normalizedNorm = normalize(fragNormal);

    float n_dot_l = max(dot(normalizedNorm, l), 0.0);

	vec3 diffuse;
	diffuse = vertexKd * n_dot_l;

    vec3 specular = vec3(0.0);

    if (n_dot_l > 0.0) {
		// Halfway vector.
		vec3 h = normalize(v + l);
        float n_dot_h = max(dot(normalizedNorm, h), 0.0);

        specular = vertexKs * pow(n_dot_h, vertexNs);
    }

    return diffuse + specular;
}

void main() {
    vec3 phongColour = phongModel(vertexPosition, vertexNormal) * lightColour;
    float shade = texture(shadow, vec3(shadowPosition.xy, shadowPosition.z - SHADOW_SAMPLE_BIAS / shadowPosition.w));

    // We do this to make shadows not shitty, this isn't a usual part of phong shading
    // We make shadows lighter where the normal is closer to our view direction
    float ambientCoefficient = dot(normalize(vertexNormal), normalize(-vertexPosition));
    vec3 ambientValue = ambientIntensity * ambientCoefficient * vertexKd;

    fragColour = vec4(phongColour * shade + ambientValue, 1.0f);
}
