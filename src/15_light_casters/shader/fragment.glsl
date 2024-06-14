#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess; //影响镜面高光的散射半径
};

//希望配置每个光源分量的权重
struct Light
{
    vec3 position; // 使用定向光就不再需要位置信息了
    vec3 direction; //聚光的方向向量，通常都在玩家的正前方
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //光线衰减系数
    float constant;
    float linear;
    float quadratic;

    //切光角
    float cutOff; //内切广角
    float outerCutOff; //外切广角

};

//观察者位置
uniform vec3 viewPos;

//材质
uniform Material material;

uniform Light light;

in vec3 Normal;
in vec3 FragPos;
in vec2 Texcoord;
out vec4 FragColor;

void main()
{
    //光的方向向量是光源位置向量与片段位置向量之间的向量差
    vec3 lightDir = normalize(light.position - FragPos);

    float theta = dot(lightDir,normalize(-light.direction));
    // 计算内外圆锥之差
    float epsilon = light.cutOff - light.outerCutOff; 
    // 计算光照,将其范围限制在0 - 1
    float intensity = clamp((theta - light.outerCutOff) / epsilon,0.0,1.0);

    //如果当前角度小于切光角，会出现聚光的效果
    //余弦值越小，角度越大
    //计算衰减系数
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                                    light.quadratic * distance * distance);
    //环境光照
    vec3 ambient = light.ambient * vec3(texture(material.diffuse,Texcoord)).rgb;
    vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(-light.direction);
    // 计算法向量和光的方向向量的点积
    // 避免角度大于90度出现点为负数的情况,使用0限制
    // 相当于用一个因子来计算漫反射光照
    float diff = max(dot(norm, lightDir),0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,Texcoord)).rgb;

    vec3 viewDir = normalize(viewPos - FragPos);

    //计算反射向量
    //reflect要求第一个向量指向片段位置,而lightdir正好相反,第二个参数是已经标准化的法向量
    vec3 reflectDir = reflect(-lightDir, norm);

    // 计算反射因子

    float spec = pow(max(dot(viewDir, reflectDir),0.0),material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular,Texcoord));

    //综合最后冯氏光照结果
    vec3 result = attenuation * (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}

//有轮廓
// void main()
// {
//     //光的方向向量是光源位置向量与片段位置向量之间的向量差
//     vec3 lightDir = normalize(light.position - FragPos);

//     float theta = dot(lightDir,normalize(-light.direction));

//     if(theta > light.cutOff)
//     {
//         //如果当前角度小于切光角，会出现聚光的效果
//         //余弦值越小，角度越大
//         //计算衰减系数
//         float distance = length(light.position - FragPos);
//         float attenuation = 1.0 / (light.constant + light.linear * distance + 
//                                         light.quadratic * distance * distance);
//         //环境光照
//         vec3 ambient = light.ambient * vec3(texture(material.diffuse,Texcoord)).rgb;
//         vec3 norm = normalize(Normal);
//         // vec3 lightDir = normalize(-light.direction);
//         // 计算法向量和光的方向向量的点积
//         // 避免角度大于90度出现点为负数的情况,使用0限制
//         // 相当于用一个因子来计算漫反射光照
//         float diff = max(dot(norm, lightDir),0.0);
//         vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,Texcoord)).rgb;

//         vec3 viewDir = normalize(viewPos - FragPos);

//         //计算反射向量
//         //reflect要求第一个向量指向片段位置,而lightdir正好相反,第二个参数是已经标准化的法向量
//         vec3 reflectDir = reflect(-lightDir, norm);

//         // 计算反射因子

//         float spec = pow(max(dot(viewDir, reflectDir),0.0),material.shininess);
//         vec3 specular = light.specular * spec * vec3(texture(material.specular,Texcoord));

//         //综合最后冯氏光照结果
//         vec3 result = attenuation * (ambient + diffuse + specular);
//         FragColor = vec4(result, 1.0);
//     }
//     else
//     {
//         // 赋予一点环境光
//         FragColor = vec4(light.ambient * vec3(texture(material.diffuse,Texcoord)).rgb,1.0f);
//     }
    
// }