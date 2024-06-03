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
|--include/
|    |--glm
|    |    |--...
|    |--shader.h
|    |--stb_image.h
|    |--glad.c
|--src/
|    |--01
|    |    |--shader
|    |    |     |--glsl文件
|    |    |--img
|    |    |    |--jpg或者png等图片
|    |    |--main.cpp
|    |--02
|    |--...
|--BUILD
|--WORKSPACE
|--......
```
源码主要存放于src文件夹中，glsl里放着色器的代码

OPENGL函数说明链接：https://docs.gl
<br>
模板链接：https://threejs.org/


### 后续问题解决
由于函数很多，想将其放到不同的文件夹下，但又想从文件最一开始根目录打开，此时涉及到编译方式，选择bazel用来编译单个文件夹

### 编译命令
进入到含有BUILD文件的根目录，运行->
```bash
bazel build //:main --define folder=02_load_texture
```

### 执行命令
```bash
./src/02_load_texture/main
```


