
### 坐标系统
```
--1.局部坐标系 -> 物体本身的顶点
            ｜
            ｜
        model matrix
            ｜
            ｜
--2.世界坐标系 -> 旋转，平移，缩放
            ｜
            ｜
        view matrix
            ｜
            ｜
--3.观察坐标系 -> 离物体的远近，距离相机的位置等
            ｜
            ｜
        projection matrix
            ｜
            ｜
--4.裁剪坐标系 -> 正视投影(不会随位置发生变化)，透视投影(远小近大)
            ｜
            ｜
          viewport
            ｜
            ｜
--5.屏幕坐标系 -> 光珊化 视口变换

顶点着色器 gl_Position = projection * view * model * vec4(Position, 1.0f);
```
