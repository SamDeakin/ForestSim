#version 330

in vec4 vertexNormal;
in vec4 vertexPosition;

out vec4 fragColour;

// Light uniforms
uniform vec3 lightDirection;
uniform vec3 lightColour;
uniform float ambientIntensity;

struct Material {
    vec3 kd;
    vec3 ks;
    float shininess;
};
uniform Material material;

vec3 phongModel(vec3 fragPosition, vec3 fragNormal) {
    // Direction from fragment to light source.
    vec3 l = -lightDirection;

    // Direction from fragment to viewer (origin - fragPosition).
    vec3 v = normalize(-fragPosition.xyz);

    float n_dot_l = max(dot(fragNormal, l), 0.0);

	vec3 diffuse;
	diffuse = material.kd * n_dot_l;

    vec3 specular = vec3(0.0);

    if (n_dot_l > 0.0) {
		// Halfway vector.
		vec3 h = normalize(v + l);
        float n_dot_h = max(dot(fragNormal, h), 0.0);

        specular = material.ks * pow(n_dot_h, material.shininess);
    }

    return ambientIntensity + (diffuse + specular);
}

void main() {
	fragColour = vec4(phongModel(vertexPosition.xyz, vertexNormal.xyz) * lightColour, 1.0f);
}
