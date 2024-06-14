#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
// 还需要知道片段的位置
out vec3 FragPos;
out vec2 Texcoord;

void main()
{
    //变换到世界坐标系
    gl_Position = projection * view * model * vec4(aPos,1.0f);
    //模型矩阵左上角3 * 3 部分的逆矩阵的转置矩阵
    //法线矩阵
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Texcoord = aTexcoord;
    FragPos = vec3(model * vec4(aPos,1.0f));
}