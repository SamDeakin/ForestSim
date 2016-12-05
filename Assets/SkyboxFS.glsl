#version 330 core

in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;
uniform vec3 sunColour;

void main()
{
    color = vec4(sunColour * texture(skybox, TexCoords).xyz, 1.0f);
}
