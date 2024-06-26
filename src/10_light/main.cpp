#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"
#include "gui.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define INPUT_PATH "./src/10_light/"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
//鼠标控制
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f; //当前针与上一针的时间差
float lastFrame = 0.0f; //上一帧的时间

//初始化鼠标位置
static float LastX = 400;
static float LastY = 300;
//初始化鼠标是否第一次进入
bool Firsemouse = true;

using namespace std;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_HEIGHT,SCREEN_WIDTH,"light",nullptr,nullptr);

    if(!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //设置视口
    glfwGetFramebufferSize(window,&SCREEN_WIDTH,&SCREEN_HEIGHT);
    glViewport(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);

    Shader ourShader((std::string)INPUT_PATH + "shader/vertex.glsl",
                (std::string)INPUT_PATH + "shader/fragment.glsl");

    Shader lightShader((std::string)INPUT_PATH + "shader/vertex_light.glsl",
                (std::string)INPUT_PATH + "shader/fragment_light.glsl");

    //创建顶点数组
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };

    // 创建顶点缓冲对象
    GLuint VAO,VBO,lightVAO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    //绑定VAO
    glBindVertexArray(VAO);
    //绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STATIC_DRAW);

    //设置顶点属性指针
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,3 * sizeof(GLfloat),(void*)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1,&lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {

        processInput(window);

        // 渲染指令
        // ...
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 计算时间
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        //lookat函数参数1.摄像机位置 2.目标位置 3.上向量
        // view = glm::lookAt(glm::vec3(camX,0.0f,camZ), glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
        ourShader.Use();
        ourShader.setVec3("objectColor", 0.0f, 0.4118f, 0.5804f);
        ourShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        // 观察矩阵和投影矩阵
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        //创建模型矩阵
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, 5 * (float)glfwGetTime(),glm::vec3(0.0f,1.0f,0.0f));
        model = glm::translate(model, glm::vec3(1.2f, 0.0f, -2.0f));
        model = glm::rotate(model, 10 * (float)glfwGetTime(),glm::vec3(0.35f,1.0f,0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        ourShader.setMat4("model", model);
        //绘制物体
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        //lightShader
        //在规定使用哪个着色器程序时一定要在循环里指定，像绑定一样
        lightShader.Use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);

        model = glm::mat4(1.0f);
        // model = glm::translate(model, glm::vec3(1.2f, 1.0f, -2.0f));
        model = glm::rotate(model, 2 * (float)glfwGetTime(),glm::vec3(0.0f,1.0f,0.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        ourShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
  glDeleteVertexArrays(1,&VAO);
  glDeleteBuffers(1,&VBO);
  glfwTerminate();

  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(FORWARD, deltaTime);
  }
  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  }
  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(LEFT, deltaTime);
  }
  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(RIGHT, deltaTime);
  }
}
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
  if(Firsemouse)
  {
    LastX = xpos;
    LastY = ypos;
    Firsemouse = false;
  }
  float xoffset = xpos - LastX;
  float yoffset = ypos - LastY; //这里要相反，y坐标从底部往顶部依次增大
  LastX = xpos;
  LastY = ypos;
  camera.ProcessMouseMovement(xoffset, yoffset,true);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}