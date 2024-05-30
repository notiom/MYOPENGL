# MYOPENGL

### 1.基础部分

B站视频：https://www.bilibili.com/video/BV11Z4y1c7so/?p=23&spm_id_from=pageDriver&vd_source=f3a0c597d5c1cd4bfb61733f60f165a3
<br>
参考文档地址: https://learnopengl-cn.github.io/intro/
<br>
视频代码仓库: https://github.com/yocover/start-learning-opengl
<br>

我使用的是Windows+Wsl虚拟机的方式配置liunx的opengl开发环境，使用opengl3.3版本
<br>
在liunx中只需要下载一个glad.c即可，其他通过安装库即可，.vscode的配置文件已上传.
<br>
文件夹分布如下
```
|.vscode
|MYOPENGL/
|--img/
|    |--...
|    |--container.jpg
|--shader/
|    |--hello.glsl
|    |--...
|--include/
|    |--glad.c
|    |--stb_image.h
|    |--shader.h
|    |--...
|--src/
|    |--main.cpp
|    |--shader.cpp
|    |--...
|--......
```
源码主要存放于src文件夹中，glsl里放着色器的代码
