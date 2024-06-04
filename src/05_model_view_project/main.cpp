/*
模型坐标
观察坐标
投影坐标
裁剪坐标
屏幕坐标
*/

/*
局部空间
世界空间
观察空间（摄像机）
裁剪空间 在范围之内变化为标准化坐标，范围之外不会被映射
*/
#include <iostream>
#include <cmath>
// GLEW
// #define GLEW_STATIC
// #include <GL/glew.h>
#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"

#define GL_SHADER_PATH "./src/05_model_view_project/shader/"

void key_callback(GLFWwindow* window,int key,int scancode,int action,int mode)
{
    // 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
    // 关闭应用程序
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window,GL_TRUE);
}
void framebuffer_size_callback(GLFWwindow* window,int width,int height)
{
    glViewport(0,0,width,height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
int main()
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 创建回调函数

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

    // 设置渲染
    // glfwGetFramebufferSize(window,&width,&height);
    glViewport(0,0,800,600);

    Shader ourShader((std::string)GL_SHADER_PATH + "vertex.glsl",
                (std::string)GL_SHADER_PATH + "fragment.glsl");

    // 顶点输入部分
    // VBO内存为X -> Y -> Z -> R -> G -> B
    // 绘制矩形
    //1.平面
    // GLfloat vertices[] = 
    // {   //位置              //颜色           //纹理坐标
    //     0.5f,  0.5f, 0.0f,  1.0f,0.0f,0.0f,  1.0f,1.0f,   //右上
    //     0.5f, -0.5f, 0.0f,  0.0f,1.0f,0.0f,  1.0f,0.0f,   //右下
    //    -0.5f, -0.5f, 0.0f,  0.0f,0.0f,1.0f,  0.0f,0.0f,   //左下
    //    -0.5f,  0.5f, 0.0f,  1.0f,1.0f,0.0f,  0.0f,1.0f,   //左上
    // };
    //立体
    GLfloat vertices[] = 
    {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
    // GLuint indices[] = 
    // {
    //     0, 1, 3, //第一个三角形
    //     1, 2, 3  //第二个三角形
    // }; //索引缓冲数组
    GLuint VBO,VAO,EBO; //无符号整型
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO); //生成一个缓冲对象
    // glGenBuffers(1,&EBO); 

       //首先绑定顶点阵列对象，然后绑定并设置顶点缓冲区和属性指针
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO); //将新创建的VBO缓冲绑定到GL_ARRAY_BUFFER上
    // GL_STATIC_DRAW ：数据不会或几乎不会改变。
    // GL_DYNAMIC_DRAW：数据会被改变很多。
    // GL_STREAM_DRAW ：数据每次绘制时都会改变。
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW); //复制数据到缓冲区

    // 通过索引缓冲数组绑定的EBO
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);  //这里要区别于VBO到VAO
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    //链接顶点属性
    //args  1.顶点属性位置值
    //      2.顶点属性大小
    //      3.参数指定数据的类型
    //      4.是否希望数据被标准化
    //      5.从这个属性第二次出现到整个数组0位置之间有多少字节，也可以设置为0让OPENGL自己计算(数据必须紧密排列)
    //      6.位置数据在缓冲中起始位置的偏移量(Offset)
    //位置属性配置
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    //颜色属性配置(现在的数组只赋值了顶点颜色，根据线性结合使其有了渐变色)
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(GLfloat),(GLvoid*)(3 * sizeof(GLfloat))); //这次有偏移量
    glEnableVertexAttribArray(1);
    //纹理属性配置
    // glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8 * sizeof(GLfloat),(GLvoid*)(6 * sizeof(GLfloat)));
    // glEnableVertexAttribArray(2);
    // glBindBuffer(GL_ARRAY_BUFFER,0);

    //解绑
    glBindVertexArray(0);

    //创建纹理
    // -------------------------
    GLuint texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1,&texture1);
    // 即将进行的 GL_TEXTURE_2D 操作现在都会对该纹理对象产生影响
    glBindTexture(GL_TEXTURE_2D,texture1);
    //设置纹理装饰器参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLfloat borderColor[] = {0.0f, 0.0f, 0.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,borderColor);
    //设置纹理过滤器参数
    //多级渐远纹理(只有在缩小时才有用，方法会报错)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // 反转图片y轴
    unsigned char *data = stbi_load("./src/05_model_view_project/img/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture container" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D,0); //解绑

    // texture 2
    // ---------
    glGenTextures(1,&texture2);
    // 即将进行的 GL_TEXTURE_2D 操作现在都会对该纹理对象产生影响
    glBindTexture(GL_TEXTURE_2D,texture2);
    //设置纹理装饰器参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //设置纹理过滤器参数
    //多级渐远纹理(只有在缩小时才有用，方法会报错)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    //加载图片
    data = stbi_load("./src/05_model_view_project/img/awesomeface.png", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture awesomeface" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D,0); //解绑

    // 在 OpenGL 中，告诉每个采样器它属于哪个纹理单元（只需执行一次)
    // -------------------------------------------------------------------------------------------
    //使用前必须激活函数
    ourShader.Use(); 
    //在此处只是分配位置值，当只有一个时，默认分配为0，但是超过2个时，需要分配位置值，且主函数中必须要激活
    glUniform1i(glGetUniformLocation(ourShader.Program, "texture1"), 0);
    glUniform1i(glGetUniformLocation(ourShader.Program, "texture2"), 1);

    //初始化projection
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f),(float)width / (float)height,0.1f,100.0f);
    glUniformMatrix4fv(glGetUniformLocation(ourShader.Program,"projection"),1,GL_FALSE,&projection[0][0]);

    //定义位移向量为立方体指定在世界空间中的位置
    glm::vec3 cubePositions[] = 
    {
        glm::vec3(0.0f,0.0f,0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    //启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        //渲染指令......
        // 设置clear颜色
        glClearColor(0.2f,0.3f,0.3f,1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清空颜色缓冲和深度缓冲


        //绑定纹理(必须)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1); //这里绑定纹理要绑定为1
        glBindTexture(GL_TEXTURE_2D, texture2);  

        ourShader.Use();
        //opengl是一个右手坐标系，但是标准化设备坐标系中实际上使用的是左手坐标系
        //初始化model，view
        // 此处初始化不能一起初始化
        // glm::mat4 model = glm::mat4(1.0f);
        // glm::mat4 view = glm::mat4(1.0f);
        // model = glm::rotate(model,(float)glfwGetTime() * glm::radians(50.0f),glm::vec3(0.5,1.0f,0.0f));
        // view = glm::translate(view,glm::vec3(-0.0f,0.0f,-3.0f)); //向场景反方向移动
        // glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"),1,GL_FALSE,glm::value_ptr(model));
        // glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "view"),1,GL_FALSE,glm::value_ptr(view));

        //绘制元素
        glBindVertexArray(VAO);
        for(GLint i = 0;i < 10;i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model,cubePositions[i]);
            model = glm::rotate(model,(float)glfwGetTime() * glm::radians(20.0f * (i % 3)),glm::vec3(0.5,1.0f,0.0f));
            glm::mat4 view = glm::mat4(1.0f); //向场景反方向移动
            view = glm::translate(view,glm::vec3(-0.0f,0.0f,-3.0f));
            glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"),1,GL_FALSE,glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "view"),1,GL_FALSE,glm::value_ptr(view));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);  //交换缓冲
        glfwPollEvents(); //检查事件
    }
    //删除缓冲
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    // glDeleteBuffers(1,&EBO);
    //双缓冲
    glfwDestroyWindow(window);
    glfwTerminate();  // 释放内存
    return 0;
}
