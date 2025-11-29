#version 330 core
out vec4 FragColor;
in vec3 WorldPos;

uniform vec3 lightPos;
uniform vec3 lightDirection;
uniform vec3 lightColor;

void main()
{
    // 计算到光线中心的距离
    vec3 toLight = WorldPos - lightPos;
    float distToAxis = length(cross(toLight, lightDirection));
    float distAlongAxis = dot(toLight, lightDirection);
    
    // 基于距离的透明度
    float alpha = 1.0 - smoothstep(0.0, 1.0, distToAxis) * 
                  smoothstep(0.0, 1.0, distAlongAxis / 10.0);
    
    // 边缘更透明
    alpha *= 0.3;
    
    FragColor = vec4(lightColor, alpha);
}