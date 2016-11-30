#version 330

in vec4 vertexColour;
in vec4 vertexNormal;
in vec4 vertexPosition;

out vec4 fragColour;

void main() {
	fragColour = vertexColour;
}
