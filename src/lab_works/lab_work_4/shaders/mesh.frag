#version 450

layout( location = 0 ) out vec4 fragColor;

uniform vec3 uAmbientColor;
uniform vec3 uDiffuse;
uniform vec3 uSpecular;
uniform float uShiny;
uniform vec3 lightPos;

in vec3 normal;
in vec4 pos;
void main()
{
	vec3 normalCheck;
	vec3 light= lightPos-pos.xyz;
	vec3 lightDirection=normalize(pos.xyz-light);
	vec3 posToCam=normalize(-pos.xyz);
	vec3 h=normalize(posToCam-lightDirection);

	if(dot(normal,posToCam)<0)
		normalCheck=-normal;
	else
		normalCheck=normal;

	vec3 res=uAmbientColor+uDiffuse*max(dot(normalCheck,normalize(light)),0.f)+uSpecular*pow(max(dot(normalCheck,h),0),uShiny);
	fragColor = vec4(res,1.f);
}
