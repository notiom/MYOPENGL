#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

void main()
{
    //环境光照
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    vec3 norm = normalize(Normal);
    //光的方向向量是光源位置向量与片段位置向量之间的向量差
    vec3 lightDir = normalize(lightPos - FragPos);
    // 计算法向量和光的方向向量的点积
    // 避免角度大于90度出现点为负数的情况,使用0限制
    // 相当于用一个因子来计算漫反射光照
    float diff = max(dot(norm, lightDir),0.0);
    vec3 diffuse = diff * lightColor;
    //定义镜面高光颜色
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);

    //计算反射向量
    //reflect要求第一个向量指向片段位置,而lightdir正好相反,第二个参数是已经标准化的法向量
    vec3 reflectDir = reflect(-lightDir, norm);

    // 计算反射因子
    //32次幂是高光的反光度,反光度越高,反射能力越强,高光电就会越小
    float spec = pow(max(dot(viewDir, reflectDir),0.0),32);
    vec3 specular = specularStrength * spec * lightColor;

    //综合最后冯氏光照结果
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}