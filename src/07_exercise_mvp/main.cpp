#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"
#include "geometry/BoxGeometry.h"
#include "geometry/PlaneGeometry.h"
#include "geometry/SphereGeometry.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "gui.h"

#define INPUT_PATH "./src/07_exercise_mvp/"
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

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
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  // 整个窗口都会被用来渲染
  glEnable(GL_PROGRAM_POINT_SIZE); 
  // 启用混合功能
  glEnable(GL_BLEND);
  // 该混合模式可以实现透明效果
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
  // glDepthFunc(GL_LESS);

  // 注册窗口变化监听
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

      Shader ourShader((std::string)INPUT_PATH + "shader/vertex.glsl",
                (std::string)INPUT_PATH + "shader/fragment.glsl");

  PlaneGeometry planeGeometry(1.0, 1.0, 1.0, 1.0);
  BoxGeometry boxGeometry(1.0, 1.0, 1.0);
  SphereGeometry sphereGeometry(0.5, 20.0, 20.0);

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
  unsigned char *data = stbi_load("./src/07_exercise_mvp/img/container.jpg", &width, &height, &nrChannels, 0);

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
  data = stbi_load("./src/07_exercise_mvp/img/awesomeface.png", &width, &height, &nrChannels, 0);

  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  stbi_image_free(data);
  ourShader.Use();
  ourShader.setInt("texture1", 0);
  ourShader.setInt("texture2", 1);

  float factor = 0.0;

  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),
      glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),
      glm::vec3(-1.3f, 1.0f, -1.5f)};

  // 观察位置的变化
  float fov = 45.0f;
  //相机的变化
  float view_translate[] = {0.0f,0.0f,-5.0f};
  ImVec4 clear_color = ImVec4(0.21, 0.3, 0.21, 1.0);
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
    ImGui::SliderFloat("fov", &fov, 0.0f, 360.0f);
    ImGui::SliderInt("width", &SCREEN_WIDTH,1,1980);
    ImGui::SliderInt("height", &SCREEN_HEIGHT,1,1080);
    ImGui::SliderFloat3("translate",&view_translate[0],-10.0f,10.0f);
    ImGui::ColorEdit3("clear color", (float *)&clear_color);
    ImGui::End();

    cout << "fov = " << fov << endl;

    // 渲染指令
    // ...
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ourShader.Use();

    factor = glfwGetTime();
    ourShader.setFloat("factor", -factor * 0.3);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(view_translate[0], view_translate[1], view_translate[2]));
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    ourShader.setMat4("view", view);
    ourShader.setMat4("projection", projection);

    glBindVertexArray(boxGeometry.VAO);

    for (unsigned int i = 0; i < 10; i++)
    {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(1.0, 1.0, 1.0));
      model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
      float angle = 20.f * i;
      model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      ourShader.setMat4("model", model);
      glDrawElements(GL_TRIANGLES, boxGeometry.indices.size(), GL_UNSIGNED_INT, 0);
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0, 0.0, 0.0));
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0));
    ourShader.setMat4("model", model);

    glBindVertexArray(planeGeometry.VAO);
    glDrawElements(GL_TRIANGLES, planeGeometry.indices.size(), GL_UNSIGNED_INT, 0);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(1.0, 0.5, 0.5));
    ourShader.setMat4("model", model);

    glBindVertexArray(sphereGeometry.VAO);
    glDrawElements(GL_TRIANGLES, sphereGeometry.indices.size(), GL_UNSIGNED_INT, 0);

    // 渲染 gui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  boxGeometry.dispose();
  planeGeometry.dispose();
  sphereGeometry.dispose();
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
}