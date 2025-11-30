#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 direction;
    vec3 color;
    float strength;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
    float softEdge;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform Material material;
uniform Light light[8];
uniform vec3 viewPos;

void main()
{    
    vec3 texDiffuse = texture(material.texture_diffuse1, TexCoords).rgb;
    vec3 texSpecular = texture(material.texture_specular1, TexCoords).rgb;
    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // 基础环境光
    vec3 ambient = vec3(0.4) * texDiffuse;
    vec3 result = ambient;
    
    for(int i = 0; i < 8; i++)
    {
        vec3 lightDir = normalize(light[i].position - FragPos);

         float theta = dot(lightDir, normalize(-light[i].direction));
         if(theta > light[i].outerCutOff) 
        {
            // 计算到光束中心线的距离
            vec3 right = normalize(cross(light[i].direction, vec3(0.0, 1.0, 0.0)));
            vec3 up = normalize(cross(right, light[i].direction));
        
            vec3 toFrag = FragPos - light[i].position;
            float distRight = dot(toFrag, right);
            float distUp = dot(toFrag, up);
        
            // 使用圆形判断
            float radialDistance = sqrt(distRight * distRight + distUp * distUp);
            float beamRadius = length(toFrag) * tan(acos(light[i].cutOff));
            float softEdgeRadius = beamRadius * (1.0 + light[i].softEdge);
        
            // 计算边缘柔和度
            float intensity = 1.0;
            if (radialDistance > beamRadius) {
                if (radialDistance < softEdgeRadius) {
                    intensity = 1.0 - (radialDistance - beamRadius) / (softEdgeRadius - beamRadius);
                } else {
                    intensity = 0.0;
                }
            }
        
            if(intensity > 0.0) 
            {
                // 计算衰减
                float distance = length(light[i].position - FragPos);
                float attenuation = 1.0 / (light[i].constant + light[i].linear * distance + light[i].quadratic * (distance * distance));
            
                // 使用color和strength计算光照
                float diff = max(dot(norm, lightDir), 0.0);
                vec3 diffuse = light[i].color * diff * texDiffuse * attenuation * intensity * light[i].strength;
        
                // specular
                vec3 reflectDir = reflect(-lightDir, norm);  
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
                vec3 specular = light[i].color * spec * texSpecular * attenuation * intensity * light[i].strength;
            
                result += diffuse + specular;
            }
        }
    }
    
    // 色调映射
    result = result / (result + vec3(1.0));
    result = pow(result, vec3(0.8));
    
    FragColor = vec4(result, 1.0);
}