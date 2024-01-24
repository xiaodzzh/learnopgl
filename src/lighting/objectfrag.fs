#version 330 core
struct Material{
	sampler2D diffuse;
    sampler2D specular;
	sampler2D emsion;
	sampler2D diftexture;
    float shininess;
};
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
in vec4 difView;
out vec4 FragColor;
uniform vec3 viewPos;
in vec3 fragPos;
in vec3 normal;
in vec2 out_texture;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec2 mixseed;
uniform bool useTexture;
uniform Material material;
uniform Light light;
void main()
{
	float distance    = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + 
            light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, out_texture));
	vec3 norNormal = normalize(normal);
	vec3 norlightDir = normalize(light.position - fragPos);
	
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-norlightDir, norNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = vec3(texture(material.specular, out_texture)) * spec * light.specular;
	
	float diff = max(dot(norlightDir, norNormal), 0.0);
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, out_texture));
	
	vec3 emsion = vec3(texture(material.emsion, out_texture));
	
	
	ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation; 
	emsion *= attenuation;
	
	
	float theta = dot(norlightDir, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outterOff;
	float intensity = clamp((theta - light.outterOff) / epsilon, 0.0, 1.0);
	
	vec2 texcoord = normalize(difView.xyz).xy;
	vec3 dift = texture(material.diftexture, (texcoord.xy) * 0.9 + 0.5).rgb * diff;
	
	dift *= intensity;
	dift *= attenuation;
	
	vec4 finalcolor = vec4((diffuse * intensity + ambient + specular * intensity +  dift), 1.0);
	//vec4 finalcolor = vec4((diffuse+ ambient + specular + emsion * intensity + dift), 1.0);
	if(useTexture)
	{
		FragColor = mix(texture(texture1, out_texture) * finalcolor, texture(texture2, out_texture) * finalcolor, mixseed.x);
	}
	else
	{
		FragColor = finalcolor;
	}
	
	//FragColor = finalcolor;
}