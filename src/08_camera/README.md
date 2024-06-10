### lookat
```
叉乘 满足右手定则，四指指向第一个向量，弯曲指向第二个向量，大拇指的方向就是叉乘结果的方向。
lookat的三个参数分别为     ->1.摄像机位置
                        ->2.目标位置
                        ->3.上向量位置
```

### 键盘控制
```
需要设置一个全局变量，调用process的回调函数处理按键信息，

叉乘 满足右手定则，四指指向第一个向量，弯曲指向第二个向量，大拇指的方向就是叉乘结果的方向。

```
### 移动速度
如果处理速度慢，即时间长，速度就应该相对快一点

### 万向结死锁
blender下载链接:https://mp.weixin.qq.com/mp/appmsgalbum?__biz=MzA4MjU4MTg2Ng==&action=getalbum&album_id=3421585741472366602

在x/z和y的平面如图所示:<br>
<img src=".img/img3.png" height="400px" width="600px"/> 
算出的方向向量的计算公式为<br>
--y = sin(pitch)<br>
--x = cos(pitch)<br>
--z = cos(pitch)<br>
<br>
将x，z记为d<br>
d在x，z投影平面上如下图所示<br>
<img src=".img/img3.png" height="400px" width="600px"/> 
算出的方向向量的计算公式为<br>
--y = sin(pitch)<br>
--x = d * cos(yaw) = cos(pitch) * cos(yaw)<br>
--z = d * sin(yaw) = cos(pitch) * sin(yaw)<br>

### 鼠标控制
--1.计算鼠标距上一帧的偏移量。
--2.把偏移量添加到摄像机的俯仰角和偏航角中。
--3.对偏航角和俯仰角进行最大和最小值的限制。
--4.计算方向向量。

### 滚轮控制
改变透视矩阵来改变观察的大小