#version 450
layout( location = 0 ) out vec4 fragColor;

layout(binding = 0) uniform sampler2D uPositionMap;
layout(binding = 1) uniform sampler2D uNormalMap;
layout(binding = 2) uniform sampler2D uAmbiantMap;
layout(binding = 3) uniform sampler2D uDiffuseMap;
layout(binding = 4) uniform sampler2D uSpecularMap;
layout(binding = 5) uniform sampler2D uOcclusion;

uniform vec3 lightPosition;

void main()
{
	ivec2 texCoords=ivec2(gl_FragCoord.xy);

	vec3 res,ambient,diffuse,specular,normalChecked,light,lightDirection,posToCam,h,posChecked,lightChecked;
	float shiny,occlusion;

	diffuse=texelFetch(uDiffuseMap,texCoords,0).xyz;
	specular=texelFetch(uSpecularMap,texCoords,0).xxx;
	normalChecked=texelFetch(uNormalMap,texCoords,0).xyz;
	posChecked=texelFetch(uPositionMap,texCoords,0).xyz;;;
	shiny=texelFetch(uSpecularMap,texCoords,0).w;
	occlusion=texelFetch(uOcclusion,texCoords,0).x;
	ambient=vec3(0.3 * diffuse * occlusion);

	lightChecked=lightPosition;
	light= normalize(lightChecked-posChecked);
	lightDirection=normalize(posChecked-lightChecked);
	posToCam=normalize(-posChecked);
	h=normalize(posToCam-lightDirection);



	res=ambient+diffuse*max(dot(normalize(normalChecked),normalize(light)),0.f)+specular*pow(max(dot(normalChecked,h),0),shiny);

	//ACES TONE MAPPING
	res=vec3(pow(res.x,1.5),pow(res.y,1.5),pow(res.z,1.5));
	float a = 2.51f;
	float b = 0.03f;
	float c = 2.43f;
	float d = 0.59f;
	float e = 0.14f;
	res=(res*(a*res+b))/(res*(c*res+d)+e);
	fragColor = vec4(res,1.f);
}