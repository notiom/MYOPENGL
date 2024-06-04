#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec3 ourPos;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixPercent;

void main() {
  // float xy = length(ourPos.xy);
  FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixPercent);
}