#version 450

layout( location = 0 ) out vec3 fragPosition;
layout( location = 1 ) out vec3 fragNormal;
layout( location = 2 ) out vec3 fragAmbiant;
layout( location = 3 ) out vec3 fragDiffuse;
layout( location = 4 ) out vec4 fragSpecular;
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

	vec3 res,ambient,diffuse,specular,normalChecked,posChecked;
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
	}
	else
	{
		normalChecked=normal;
		posChecked=pos.xyz;
	}

	if(uHasShininessMap)
		shiny=texture(uShininessMap,texCoords).x;
	else
		shiny=uShiny;

	fragPosition=posChecked;
	fragNormal=normalChecked;
	fragAmbiant=ambient;
	fragDiffuse=diffuse;
	fragSpecular=vec4(specular,shiny);
}