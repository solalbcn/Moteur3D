#version 450
layout(location = 0 ) in vec4 aVertexPosition;
layout(location = 1 ) in vec4 aVertexColor;
uniform float uTranslationX;
out vec4 laCouleur;
void main() {
	laCouleur=aVertexColor;
	gl_Position=aVertexPosition;
	gl_Position.x+=uTranslationX;
}
