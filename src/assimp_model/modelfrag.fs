#version 330 core

struct Light{
	vec3 position;
	vec3 ambient;
	vec3 direction;
	vec3 diffuse;
	vec3 specular;
	float constant;
    float linear;
    float quadratic;
	float cutOff;
	float outterOff;
};

out vec4 FragColor;

uniform vec3 viewPos;
in vec3 fragPos;
in vec3 normal;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

uniform sampler2D texture_specular1;

uniform Light light;

void main()
{    
    //FragColor = texture(texture_diffuse1, TexCoords);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
	vec3 norNormal = normalize(normal);
	vec3 norlightDir = normalize(light.position - fragPos);

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-norlightDir, norNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = vec3(texture(texture_specular1, TexCoords)) * spec * light.specular;

	float diff = max(dot(norlightDir, norNormal), 0.0);
	vec3 diffuse = diff * light.diffuse * vec3(texture(texture_diffuse1, TexCoords));

	//vec3 emsion = vec3(texture(material.emsion, out_texture));//¿‡À∆¿ÿ…‰—€µƒπ‚


	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	//emsion *= attenuation;


	float theta = dot(norlightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outterOff;
	float intensity = clamp((theta - light.outterOff) / epsilon, 0.0, 1.0);


	vec4 finalcolor = vec4((diffuse * intensity + ambient + specular * intensity), 1.0);

	FragColor = finalcolor;
}