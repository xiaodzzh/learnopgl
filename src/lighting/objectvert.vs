#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;
layout (location = 2) in vec3 aNormal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 normal;
out vec2 out_texture;
out vec3 fragPos;
void main()
{
	fragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(fragPos, 1.0);
	//normal = aNormal;
	normal = mat3(transpose(inverse(model))) * aNormal;
	out_texture = aTexture;
};