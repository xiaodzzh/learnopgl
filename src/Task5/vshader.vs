
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 my_color;
uniform vec4 clear_color;
uniform vec2 offset;
out vec4 our_color;
void main()
{
    gl_Position = vec4(aPos.x + offset.x, aPos.y - offset.y, aPos.z, 1);
	our_color = my_color;
};