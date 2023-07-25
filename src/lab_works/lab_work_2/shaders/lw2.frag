#version 450
layout( location = 0 ) out vec4 frag_color;
uniform float light;
in vec4 laCouleur;
void main() {
frag_color=laCouleur*light;
}