#version 450
layout( location = 0 ) out float fragColor;

layout(binding = 0) uniform sampler2D uPositionMap;
layout(binding = 1) uniform sampler2D uNormalMap;
layout(binding = 2) uniform sampler2D uTexNoise;
uniform vec3 samples[64];
uniform mat4 projection;
uniform float radius;
uniform float bias;
uniform float power;
void main()
{
    ivec2 TexCoords = ivec2(gl_FragCoord.xy);
	float occlusion = 0.0;
	float sampleDepth;
	float rangeCheck;
	vec4 offset;
	vec3 samplePos;

	vec3 fragPos   = texelFetch(uPositionMap, TexCoords,0).xyz;
	vec3 normal    = texelFetch(uNormalMap, TexCoords,0).rgb;
	vec3 randomVec = texture(uTexNoise, TexCoords).xyz;  
	vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN       = mat3(tangent, bitangent, normal); 

	for(int i = 0; i < samples.length(); ++i)
	{
		// get sample position
		samplePos = TBN * samples[i]; // from tangent to view-space
		samplePos = fragPos + samplePos * radius; 
		offset = vec4(samplePos, 1.0);
		offset      = projection * offset;    // from view to clip-space
		offset.xyz /= offset.w;               // perspective divide
		offset.xyz  = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
		sampleDepth = texture(uPositionMap, offset.xy).z; 
		rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0)*rangeCheck; 
	}
	occlusion = 1.0 - (occlusion / samples.length());
	fragColor = pow(occlusion,power);
}