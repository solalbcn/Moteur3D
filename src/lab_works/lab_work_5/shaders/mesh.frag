#version 450

layout( location = 0 ) out vec4 fragColor;
layout(binding = 0) uniform sampler2D uAmbiantMap;
layout(binding = 1) uniform sampler2D uDiffuseMap;
layout(binding = 2) uniform sampler2D uSpecularMap;
layout(binding = 3) uniform sampler2D uShininessMap;
layout(binding = 4) uniform sampler2D uNormalsMap;

uniform bool uHasAmbiantMap;
uniform bool uHasDiffuseMap;
uniform bool uHasSpecularMap;
uniform bool uHasShininessMap;
uniform bool uHasNormalsMap;
uniform vec3 uAmbientColor;
uniform vec3 uDiffuse;
uniform vec3 uSpecular;
uniform float uShiny;


in vec3 lightPos;
in vec2 texCoords;
in vec3 normal;
in vec3 tanPos;
in vec3 tanLight;
in vec4 pos;

void main()
{
		if(texture(uDiffuseMap,texCoords).w<0.5)
		discard;

	vec3 res,ambient,diffuse,specular,normalChecked,posChecked,posToCam,h,posCheck,lightChecked,light,lightDirection;
	float shiny;

	if(uHasDiffuseMap)
		diffuse=texture(uDiffuseMap,texCoords).xyz;
	else
		diffuse=uDiffuse;

	if(uHasAmbiantMap)
		ambient=texture(uAmbiantMap,texCoords).xyz;
	else
		ambient=uAmbientColor;

	if(uHasSpecularMap)
		specular=texture(uSpecularMap,texCoords).xxx;
	else
		specular=uSpecular;

	if(uHasNormalsMap)
	{
		normalChecked=texture(uNormalsMap,texCoords).xyz;
		normalChecked=normalize(normalChecked*2.0-1.0);
		posChecked=tanPos;
		lightChecked=tanLight;
	}
	else
	{
		normalChecked=normal;
		posChecked=pos.xyz;
		lightChecked=tanLight;
	}

	if(uHasShininessMap)
		shiny=texture(uShininessMap,texCoords).x;
	else
		shiny=uShiny;

	if(dot(normalChecked,posToCam)<0)
		normalChecked=-normalChecked;
	else
		normalChecked=normalChecked;

	light= normalize(lightChecked-posChecked);
	lightDirection=normalize(posChecked-lightChecked);
	posToCam=normalize(-posChecked);
	h=normalize(posToCam-lightDirection);
	res=ambient+diffuse*max(dot(normalize(normalChecked),normalize(light)),0.f)+specular*pow(max(dot(normalChecked,h),0),shiny);
	fragColor = vec4(res,1.f);
}
