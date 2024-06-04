#version 330 core

layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
// layout (location = 2) in vec2 aTexCoord;
layout (location = 1) in vec2 aTexCoord;

// out vec3 ourColor;
out vec2 ourTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //乘法从右像左读
    gl_Position = projection * view * model * vec4(aPos,1.0f);
    // ourColor = aColor;
    ourTexCoord = aTexCoord;
}