#version 450
layout( location = 0 ) out vec4 fragColor;

layout(binding = 0) uniform sampler2D uPositionMap;
layout(binding = 1) uniform sampler2D uNormalMap;
layout(binding = 2) uniform sampler2D uAmbiantMap;
layout(binding = 3) uniform sampler2D uDiffuseMap;
layout(binding = 4) uniform sampler2D uSpecularMap;

uniform vec3 lightPosition;

void main()
{
	ivec2 texCoords=ivec2(gl_FragCoord.xy);

	vec3 res,ambient,diffuse,specular,normalChecked,light,lightDirection,posToCam,h,posChecked,lightChecked;
	float shiny;
	diffuse=texelFetch(uDiffuseMap,texCoords,0).xyz;
	ambient=texelFetch(uAmbiantMap,texCoords,0).xyz;
	specular=texelFetch(uSpecularMap,texCoords,0).xxx;
	normalChecked=texelFetch(uNormalMap,texCoords,0).xyz;
	posChecked=texelFetch(uPositionMap,texCoords,0).xyz;;;
	shiny=texelFetch(uSpecularMap,texCoords,0).w;
	
	lightChecked=lightPosition;
	light= normalize(lightChecked-posChecked);
	lightDirection=normalize(posChecked-lightChecked);
	posToCam=normalize(-posChecked);
	h=normalize(posToCam-lightDirection);

	if(dot(normalChecked,posToCam)<0)
		normalChecked=-normalChecked;
	else
		normalChecked=normalChecked;

	res=ambient+diffuse*max(dot(normalize(normalChecked),normalize(light)),0.f)+specular*pow(max(dot(normalChecked,h),0),shiny);
	fragColor = vec4(res,1.f);
}