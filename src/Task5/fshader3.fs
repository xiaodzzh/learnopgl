#version 330 core
out vec4 FragColor;
uniform vec4 clear_color;
uniform vec2 mixseed;
in vec2 out_texture;
uniform sampler2D texture1;
uniform sampler2D texture2;
void main()
{
   FragColor = mix(texture(texture1, out_texture) * clear_color, texture(texture2, out_texture) * clear_color, mixseed.x);
}