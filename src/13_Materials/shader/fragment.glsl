#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess; //影响镜面高光的散射半径
};

//希望配置每个光源分量的权重
struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//观察者位置
uniform vec3 viewPos;

//材质
uniform Material material;

uniform Light light;

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

void main()
{
    //环境光照
    vec3 ambient = light.ambient * material.ambient;
    vec3 norm = normalize(Normal);
    //光的方向向量是光源位置向量与片段位置向量之间的向量差
    vec3 lightDir = normalize(light.position - FragPos);
    // 计算法向量和光的方向向量的点积
    // 避免角度大于90度出现点为负数的情况,使用0限制
    // 相当于用一个因子来计算漫反射光照
    float diff = max(dot(norm, lightDir),0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    vec3 viewDir = normalize(viewPos - FragPos);

    //计算反射向量
    //reflect要求第一个向量指向片段位置,而lightdir正好相反,第二个参数是已经标准化的法向量
    vec3 reflectDir = reflect(-lightDir, norm);

    // 计算反射因子

    float spec = pow(max(dot(viewDir, reflectDir),0.0),material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    //综合最后冯氏光照结果
    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}