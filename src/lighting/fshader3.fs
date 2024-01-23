#version 330 core
out vec4 FragColor;
uniform vec4 lightcolor;
uniform vec4 objectcolor;
uniform vec2 mixseed;
in vec2 out_texture;
uniform sampler2D texture1;
uniform sampler2D texture2;
void main()
{
	vec4 color = vec4(lightcolor * objectcolor);
	FragColor = mix(texture(texture1, out_texture) * color, texture(texture2, out_texture) * color, mixseed.x);
}