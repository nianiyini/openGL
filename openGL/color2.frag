#version 460 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// 包含光照函数库
//#include "all_light.glsl"

// 材质属性
uniform Material material;
uniform vec3 viewPos;

void main()
{
    // 属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // 第一阶段：方向光照
    vec3 result = CalcDirLight(dirLight, material, norm, viewDir, TexCoords);
    
    // 第二阶段：点光源
    for(int i = 0; i < numPointLights; i++)
        result += CalcPointLight(pointLights[i], material, norm, FragPos, viewDir, TexCoords);
    
    // 第三阶段：聚光
    for(int i = 0; i < numSpotLights; i++)
        result += CalcSpotLight(spotLights[i], material, norm, FragPos, viewDir, TexCoords);
    
    FragColor = vec4(result, 1.0);
}