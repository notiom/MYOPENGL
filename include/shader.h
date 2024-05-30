#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

class Shader
{
public:
    //程序id
    GLuint Program;
    //构造函数
    Shader(const GLchar* vertexPath,const GLchar* fragmentPath);
    //使用程序
    void Use();
};

#endif
