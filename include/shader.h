#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
public:
    //程序Program
    GLuint Program;
    //构造函数
    Shader(const std::string& vertexPath,const std::string& fragmentPath);
    //使用程序
    void Use();
    //uniform工具函数
    void SetBool(const std::string& name,bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
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

void Shader::SetBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(this->Program, name.c_str()), (int)value);
}

// ------------------------------------------------------------------------
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(this->Program, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(this->Program, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
     glUniform2fv(glGetUniformLocation(this->Program, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(this->Program, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(this->Program, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(this->Program, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(this->Program, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
     glUniform4f(glGetUniformLocation(this->Program, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(this->Program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// -----------------------------------------------------------------------
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(this->Program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(this->Program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
#endif