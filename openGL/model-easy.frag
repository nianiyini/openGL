#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; 

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Light {
    vec3 Position;
    vec3 Color;
};

uniform Material material;
uniform Light lights[8];
uniform vec3 viewPos;

void main()
{           
    // 使用 material 结构体中的纹理，而不是未定义的 diffuseTexture
    vec3 color = texture(material.texture_diffuse1, TexCoords).rgb;
    vec3 normal = normalize(Normal);
    
    // ambient - 可以适当增加环境光
    vec3 ambient = 0.1 * color;
    
    // lighting
    vec3 lighting = vec3(0.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    for(int i = 0; i < 4; i++)
    {
        // diffuse
        vec3 lightDir = normalize(lights[i].Position - FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = lights[i].Color * diff * color;
        
        // specular (可选添加)
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = lights[i].Color * spec * texture(material.texture_specular1, TexCoords).rgb;
        
        vec3 result = diffuse + specular;
        
        // attenuation - 使用更合理的衰减公式
        float distance = length(FragPos - lights[i].Position);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
        result *= attenuation;
        
        lighting += result;
    }
    
    vec3 result = ambient + lighting;
    
    // check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(result, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
        
    FragColor = vec4(result, 1.0);
}