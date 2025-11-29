#version 330 core
out vec4 FragColor;
in vec3 Color;

void main()
{
    // 让点精灵呈现圆形而不是方形
    vec2 coord = gl_PointCoord - vec2(0.5);
    if(length(coord) > 0.5)
        discard;
        
    FragColor = vec4(Color, 1.0);
}