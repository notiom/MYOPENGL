### 复习
```
-- opengl 函数布局和输出图像API的规范

-- glad 拓展加载库，用来加载并设定opengl的指针

-- 视口 viewport 渲染时需要输入
glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
才可以完美的适配屏幕

-- 图形管线：顶点在呈现为像素前经历的全部过程
顶点数组 -> 顶点着色器 -> 图元装配 -> 几何着色器 -> 裁剪(经过裁剪坐标系) -> 光珊化  -> 片段着色器 -> 测试和混合

-- 着色器 in out 顶点着色器必须输出一个gl_Positon的位置向量，片段着色器输出一个vec4类型的颜色向量

-- 标准化设备坐标  ->裁剪与透视除法后最终呈现的坐标系

-- 顶点缓冲对象 -> 存储顶点数据 VBO
-- 顶点数组对象 -> 存储缓冲区和顶点属性状态 VAO
-- 索引缓冲对象 -> 存储索引值供后续使用

-- uniform -> 特殊的GLSL变量，全局使用

-- 纹理 -> 包裹着物体的特殊类型图像，给物体显示精细的视觉效果

-- 纹理缠绕 -> 当纹理超过范围时opengl如何采样纹理模式 repeat repeat_mirror edge edge boarder 
-- 纹理过滤 -> 有多种纹素时指定opengl如何采样 nearset linear
-- 多级渐远纹理 -> 远小近大的纹理显示效果

-- stb_image.h -> 图形加载库

-- 纹理单元 绑定纹理到不同纹理单元从而允许在用一个对象上渲染

-- GLM 为opengl打造的数学库

-- 坐标系
    --1.局部坐标系
    --2.世界坐标系 model矩阵
    --3.观察坐标系 lookat矩阵
    --4.裁剪坐标系 projection矩阵，透视投影 perspective
    --5.屏幕坐标系 viewport

-- 欧拉角 pitch yaw roll三个角构造3D方向
```

