#version 450

layout(binding = 0) uniform sampler2D ssaoInput;
layout( location = 0 ) out float fragColor;

void main() {
    ivec2 TexCoords = ivec2(gl_FragCoord.xy);
    vec2 texelSize = 1.0 / vec2(textureSize(ssaoInput, 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texelFetch(ssaoInput, ivec2(TexCoords + offset),0).r;
        }
    }
    fragColor = result / (4.0 * 4.0);
}  