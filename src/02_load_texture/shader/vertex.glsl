#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    // exercise2 :  角落放置4个笑脸
    // 图像从0 - 1 采样
    // 但是纹理坐标为0 - 2
    //若想使其居中，需要左移纹理坐标才可以
	// TexCoord = vec2(aTexCoord.x, aTexCoord.y) * 2.0f - 0.5f;
}