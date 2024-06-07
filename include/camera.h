#ifndef _CAMERA_H
#define _CAMERA_H

#include "glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_MoveMent
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};
//相机默认参数
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera
{
public:
    //相机属性
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right; //计算右向量的目的是后续计算wasd移动时需要用到右向量
    glm::vec3 WorldUp; //一开始提供的上向量的值

    //角度
    float Yaw;
    float Pitch;

    //相机选项
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    //构造函数
    Camera(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f),
            glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f),
            float yaw = YAW,
            float pitch = PITCH) : Front(glm::vec3(0.0f,0.0f,-1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
            {
                Position = position;
                WorldUp = up;
                Yaw = yaw;
                Pitch = pitch;
                updateCameraVectors();
            }
    //重构构造函数
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    
    //获取观察矩阵
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    //处理键盘输入
    void ProcessKeyboard(Camera_MoveMent direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if(direction == FORWARD)
            Position += Front * velocity;
        if(direction == BACKWARD)
            Position -= Front * velocity;
        if(direction == LEFT)
            Position -= Right * velocity;
        if(direction == RIGHT)
            Position += Right * velocity;
    }

    //处理鼠标输入
    void ProcessMouseMovement(float xoffset, float yoffset,bool constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        //确保当俯仰角（pitch）超出范围时，屏幕不会翻转
        if(constrainPitch)
        {
            if(Pitch > 89.0f)
                Pitch = 89.0f;
            if(Pitch < -89.0f)
                Pitch = -89.0f;
        }
        //更新坐标
        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if(Zoom < 1.0f)
        {
            Zoom = 1.0f;
        }
        if(Zoom > 45.0f)
        {
            Zoom = 45.0f;
        }
    }
private:
    void updateCameraVectors()
    {
    //计算front向量
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    //重新计算右向量和上向量
    Right = glm::normalize(glm::cross(Front,WorldUp));
    Up    = glm::normalize(glm::cross(Right,Front));
    }
};

#endif