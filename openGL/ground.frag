#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 WorldPos;

uniform sampler2D groundTexture;
uniform vec3 cameraPos;

void main()
{
    // 基于世界坐标计算UV，实现无缝重复
    vec2 groundUV = WorldPos.xz * 0.1; // 调整这个系数控制纹理密度
    
    // 使用fract实现无限重复
    groundUV = fract(groundUV);
    
    vec4 color = texture(groundTexture, groundUV);
    
    // 添加一些距离雾效
    float distance = length(cameraPos - WorldPos);
    float fog = exp(-distance * 0.001);
    color.rgb = mix(vec3(0.5, 0.5, 0.5), color.rgb, fog);
    
    FragColor = color;
}