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
    Shader(const std::string& vertexPath,const std::string& fragmentPath);
    //使用程序
    void Use();
};

#include "include/shader.h"

Shader::Shader(const std::string& vertexPath,const std::string& fragmentPath)
{
     // 1. 从文件路径中获取顶点/片段着色器
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // 保证ifstream可以抛出异常
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        //打开文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        //读取文件的缓冲内容到流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        //关闭文件处理器
        vShaderFile.close();
        fShaderFile.close();
        //转换流至GLchar数组
        vertexCode = vShaderStream.str();
        std::cout << "Read vertexshader code from " << vertexPath << ":\n" << vertexCode << std::endl; // Add this line for debugging
        fragmentCode = fShaderStream.str();
        std::cout << "Read fragmentshader code from " << fragmentPath << ":\n" << fragmentCode << std::endl; // Add this line for debugging
    }
    catch(const std::ifstream::failure e)
    {
        std::cerr <<  "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << e.what() << std::endl;
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();
    // 2. 编译着色器
    GLuint vertex,fragment;
    GLint success;
    GLchar infoLog[512];
    //顶点着色器
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex,1,&vShaderCode,nullptr);
    glCompileShader(vertex);
    //检查编译是否成功
    glGetShaderiv(vertex,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(vertex,512,nullptr,infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //片段着色器
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment,1,&fShaderCode,nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(fragment,512,nullptr,infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //着色器程序
    this->Program = glCreateProgram();
    glAttachShader(this->Program,vertex);
    glAttachShader(this->Program,fragment);
    glLinkProgram(this->Program);

    glGetShaderiv(this->Program,GL_LINK_STATUS,&success);
    if(!success)
    {
        glGetProgramInfoLog(this->Program,512,nullptr,infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    //删除着色器
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use()
{
    glUseProgram(this->Program);
}

#endif