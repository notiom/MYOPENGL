#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform sampler2D texture2;

out vec4 FragColor;
in vec2 outTexCoord;

void main()
{
    FragColor = texture(texture2, outTexCoord);
}