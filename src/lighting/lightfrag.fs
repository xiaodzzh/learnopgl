
#version 330 core
out vec4 FragColor;
uniform vec3 outLightcolor;
void main()
{
    FragColor = vec4(outLightcolor, 1.0);
};