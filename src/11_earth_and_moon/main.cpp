#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"
#include "gui.h"
#include "camera.h"
#include "geometry/BoxGeometry.h"
#include "geometry/PlaneGeometry.h"
#include "geometry/SphereGeometry.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define INPUT_PATH "./src/11_earth_and_moon/"
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

int main(int argc, char *argv[])
{
    glfwInit();
    // 设置主要和次要版本
    const char *glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口对象
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // -----------------------
    // 创建imgui上下文
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    // 设置样式
    ImGui::StyleColorsDark();
    // 设置平台和渲染器
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // -----------------------

    // 设置视口
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    // 整个窗口都会被用来渲染
    glEnable(GL_PROGRAM_POINT_SIZE); 
    // 启用混合功能
    glEnable(GL_BLEND);
    // 该混合模式可以实现透明效果
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    // glDepthFunc(GL_LESS);
    //不显示光标，也不会离开窗口
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //注册鼠标移动回调函数
    glfwSetCursorPosCallback(window, mouse_callback);
    // 注册窗口变化监听
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //注册鼠标滚轮回调函数
    glfwSetScrollCallback(window, scroll_callback);

    Shader earthShader((std::string)INPUT_PATH + "shader/vertex_earth.glsl",
                (std::string)INPUT_PATH + "shader/fragment_earth.glsl");

    Shader moonShader((std::string)INPUT_PATH + "shader/vertex_moon.glsl",
                (std::string)INPUT_PATH + "shader/fragment_moon.glsl");

    Shader lightShader((std::string)INPUT_PATH + "shader/vertex_light.glsl",
                (std::string)INPUT_PATH + "shader/fragment_light.glsl");

    //球形地球月球模型
    SphereGeometry sphereGeometry(0.5, 20.0, 20.0);
    //盒子光源模型
    BoxGeometry boxGeometry(1.0, 1.0, 1.0);

    // 生成纹理
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // 设置环绕和过滤方式
    float borderColor[] = {0.3f, 0.1f, 0.7f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 图像y轴翻转
    stbi_set_flip_vertically_on_load(true);

    // 加载图片
    int width, height, nrChannels;
    unsigned char *data = stbi_load("./src/11_earth_and_moon/img/earth.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // 设置环绕和过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 加载图片
    data = stbi_load("./src/11_earth_and_moon/img/moon.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
    earthShader.Use();
    //设置位置
    earthShader.setInt("texture1", 0);
    moonShader.Use();
    //设置位置
    moonShader.setInt("texture2", 0);

    float f = 0.0f;
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // 开始dear imgui 帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 创建一个窗口
        ImGui::Begin("imgui!");
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float *)&clear_color);
        ImGui::End();

        cout << "f = " << f << endl;

        // 渲染指令
        // ...
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 计算时间
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        //lookat函数参数1.摄像机位置 2.目标位置 3.上向量
        // view = glm::lookAt(glm::vec3(camX,0.0f,camZ), glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
        // 绑定地球纹理
        earthShader.Use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        earthShader.setVec3("objectColor", 0.0f, 0.4118f, 0.5804f);
        earthShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        // 观察矩阵和投影矩阵
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        earthShader.setMat4("projection", projection);
        earthShader.setMat4("view", view);

        //创建模型矩阵
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, 2 * (float)glfwGetTime(),glm::vec3(0.0f,1.0f,0.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        earthShader.setMat4("model", model);
        //绘制地球
        glBindVertexArray(sphereGeometry.VAO);
        glDrawElements(GL_TRIANGLES, sphereGeometry.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // 绑定月球纹理
        moonShader.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture2);

        moonShader.setVec3("objectColor", 0.0f, 0.4118f, 0.5804f);
        moonShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        // 观察矩阵和投影矩阵
        moonShader.setMat4("projection", projection);
        moonShader.setMat4("view", view);

        //创建模型矩阵
        model = glm::mat4(1.0f);
        model = glm::rotate(model, 2 * (float)glfwGetTime(),glm::vec3(0.0f,1.0f,0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
        model = glm::rotate(model, 6 * (float)glfwGetTime(),glm::vec3(0.0f,1.0f,0.0f));
        model = glm::scale(model, glm::vec3(0.3f));
        moonShader.setMat4("model", model);

        //绘制月球
        glBindVertexArray(sphereGeometry.VAO);
        glDrawElements(GL_TRIANGLES, sphereGeometry.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //lightShader
        //在规定使用哪个着色器程序时一定要在循环里指定，像绑定一样
        lightShader.Use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5.0f, 1.0f, -5.0f));
        model = glm::scale(model, glm::vec3(0.1f));
        lightShader.setMat4("model", model);

        glBindVertexArray(boxGeometry.VAO);
        glDrawElements(GL_TRIANGLES, boxGeometry.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // 渲染 gui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    sphereGeometry.dispose();
    boxGeometry.dispose();
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