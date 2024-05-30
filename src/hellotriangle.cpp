//
// 顶点数组 -- 顶点着色器 -- 图元装配  -- 几何着色器  --  光栅化  --裁剪  -- -- 片段着色器  --  混合与测试（viewport）
#include <iostream>
#include <cmath>
// GLEW
// #define GLEW_STATIC
// #include <GL/glew.h>
#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>

#include "include/shader.h"

void key_callback(GLFWwindow* window,int key,int scancode,int action,int mode)
{
    // 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
    // 关闭应用程序
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window,GL_TRUE);
}
int main2()
{
    //初始化窗口
    glfwInit();
    //设置版本 主要版本和次要版本 和使用内核
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE); //核心模式

    //创建窗口对象
    GLFWwindow* window = glfwCreateWindow(800,600,"learnOPENGL",NULL,NULL);
    if(window == NULL)
    {
        //cerr无缓冲区，即时输出
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 创建window的上下文，此处必须在加载gladloader之前
    glfwMakeContextCurrent(window);
     //设置回调函数
    glfwSetKeyCallback(window, key_callback);  // 创建回调函数

    // 初始化glad的函数指针
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {

        std::cerr<<"Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    // glewExperimental = GL_TRUE;
    // glewInit();
    //获取opengl的版本
    const GLubyte* version = glGetString(GL_VERSION);
    if(version)
    {
        std::cout<<"OpenGl Version: " << *version << std::endl;
    }
    else
    {
        //获取失败
        std::cerr << "Failed to retrieve OpenGL version" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    int width,height;

    //设置渲染
    glfwGetFramebufferSize(window,&width,&height);
    glViewport(0,0,width,height);

    Shader ourShader("/home/xl/src/cpp/OPENGL/shader/vertex_hello.glsl",
                "/home/xl/src/cpp/OPENGL/shader/fragment_hello.glsl");

    // 顶点输入部分
    // VBO内存为X -> Y -> Z -> R -> G -> B
    GLfloat vertices[] = 
    {   //位置             //颜色
        -0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f, //右下
        0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,  //左下
        0.0f, 0.5f, 0.0f, 0.0f,0.0f,1.0f    //顶部
    };
    GLfloat texCoords[] =
    {
        0.0f,0.0f, //左下角
        1.0f,0.0f, //右下角
        0.5f,1.0f  //上中
    };
    GLfloat boarderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,boarderColor);
    GLuint VBO,VAO; //无符号整型
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO); //生成一个缓冲对象
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO); //将新创建的VBO缓冲绑定到GL_ARRAY_BUFFER上
    // GL_STATIC_DRAW ：数据不会或几乎不会改变。
    // GL_DYNAMIC_DRAW：数据会被改变很多。
    // GL_STREAM_DRAW ：数据每次绘制时都会改变。
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW); //复制数据到缓冲区

    //链接顶点属性
    //args  1.顶点属性位置值
    //      2.顶点属性大小
    //      3.参数指定数据的类型
    //      4.是否希望数据被标准化
    //      5.从这个属性第二次出现到整个数组0位置之间有多少字节，也可以设置为0让OPENGL自己计算(数据必须紧密排列)
    //      6.位置数据在缓冲中起始位置的偏移量(Offset)
    //位置属性配置
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    //颜色属性配置(现在的数组只赋值了顶点颜色，根据线性结合使其有了渐变色)
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(GLfloat),(GLvoid*)(3 * sizeof(GLfloat))); //这次有偏移量
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    //解绑
    glBindVertexArray(0);


    //主循环
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents(); //检查事件
        // 设置clear颜色
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        //渲染指令......
        glClear(GL_COLOR_BUFFER_BIT); //清空颜色缓冲
        //激活程序对象
        ourShader.Use(); 
        //改变unifrom的值
        /*
        GLfloat timeValue = glfwGetTime();
        // std::cout<<timeValue<<std::endl;
        GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
        GLint vertexColorLocation = glGetUniformLocation(shaderProgram,"ourColor");
        glUniform4f(vertexColorLocation,0.0f,greenValue,0.0f,1.0f);
        */
        //绘制三角形
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
        glBindVertexArray(0);
        glfwSwapBuffers(window);  //交换缓冲
    
    }

    //删除缓冲
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    //双缓冲
    glfwDestroyWindow(window);
    glfwTerminate();  // 释放内存
    return 0;
}
