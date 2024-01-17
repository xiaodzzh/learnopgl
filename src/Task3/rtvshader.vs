
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexture;
uniform vec2 offset;
out vec2 out_texture;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1);
	out_texture = aTexture;
};