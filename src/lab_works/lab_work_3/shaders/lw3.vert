#version 450
layout(location = 0 ) in vec3 aVertexPosition;
layout(location = 1 ) in vec4 aVertexColor;
uniform float uTranslationX;
uniform mat4 mvpMatrix;
out vec4 laCouleur;
void main() {
	laCouleur=aVertexColor;
	gl_Position= mvpMatrix*vec4(aVertexPosition,1.f);
}
