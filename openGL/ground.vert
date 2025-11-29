#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 WorldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

void main()
{
    // 将地面放大并跟随相机（实现无限远效果）
    vec3 scaledPos = aPos * 1000.0; // 放大1000倍
    scaledPos.xz += cameraPos.xz;   // 跟随相机在XZ平面移动
    scaledPos.y = -10.0; 

    WorldPos = vec3(model * vec4(scaledPos, 1.0));
    TexCoord = aTexCoord;
    gl_Position = projection * view * vec4(WorldPos, 1.0);
}