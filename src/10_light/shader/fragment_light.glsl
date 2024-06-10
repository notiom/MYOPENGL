#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;

out vec4 Color;

void main()
{
    Color = vec4(objectColor * lightColor, 1.0f);
}