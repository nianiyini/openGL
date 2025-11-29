// lighting_functions.glsl
#ifndef LIGHTING_FUNCTIONS_GLSL
#define LIGHTING_FUNCTIONS_GLSL

// 在头文件中定义Material结构体
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

// 统一的光线数据结构 - 必须与C++结构体完全匹配
struct PointLight {
    vec3 position;
    float padding0;
    vec3 diffuse;
    float padding1;
    vec3 specular;
    float padding2;
    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 direction;
    float padding0;
    vec3 diffuse;
    float padding1;
    vec3 specular;
    float padding2;
};

struct SpotLight {
    vec3 position;
    float padding0;
    vec3 direction;
    float padding1;
    vec3 diffuse;
    float padding2;
    vec3 specular;
    float padding3;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

// 统一缓冲区 - 绑定到位置1
layout (std140, binding = 1) uniform LightingData {
    PointLight pointLights[4];
    DirLight dirLight;
    SpotLight spotLights[2];
    vec3 ambientColor;
    float padding0;
    int numPointLights;
    int numSpotLights;
};

// 点光源计算函数
vec3 CalcPointLight(PointLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoords)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                     light.quadratic * (distance * distance));    
                     
    vec3 ambient = ambientColor * vec3(texture(material.diffuse, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

// 方向光计算函数
vec3 CalcDirLight(DirLight light, Material material, vec3 normal, vec3 viewDir, vec2 texCoords)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    vec3 ambient = ambientColor * vec3(texture(material.diffuse, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));
    
    return (ambient + diffuse + specular);
}

// 聚光灯计算函数
vec3 CalcSpotLight(SpotLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoords)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction)); 
    
    if(theta > light.cutOff) 
    {    
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, normal);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + 
                         light.quadratic * (distance * distance));
        
        vec3 ambient = ambientColor * texture(material.diffuse, texCoords).rgb;
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoords).rgb;  
        vec3 specular = light.specular * spec * texture(material.specular, texCoords).rgb;
        
        diffuse *= attenuation;
        specular *= attenuation;   
        
        return (ambient + diffuse + specular);
    }
    else 
    {
        return ambientColor * texture(material.diffuse, texCoords).rgb;
    }
}

#endif