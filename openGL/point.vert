#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 Color;

uniform mat4 projection;
uniform mat4 view;
uniform float time;

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);
    Color = aColor;
    
    // 动态点大小：基于时间和位置
    float pulse = sin(time * 2.0 + aPos.x * 5.0) * 0.5 + 0.5;
    gl_PointSize = 20.0 + pulse * 30.0;  // 大小在20-50像素间脉动
}