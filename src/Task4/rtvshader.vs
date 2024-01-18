
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexture;
uniform mat4 transform;
uniform mat4 rotTransform;
uniform mat4 translateform;
uniform vec2 offset;
out vec2 out_texture;
vec4 tem;
void main()
{
    tem = transform * vec4(aPos.x, aPos.y, aPos.z, 1);
	gl_Position = rotTransform * vec4(tem.x, tem.y, tem.z, tem.w);
	gl_Position = translateform * vec4(gl_Position);
	out_texture = aTexture;
};