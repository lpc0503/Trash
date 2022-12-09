#version 330 core

in vec4 vs_vertexColor;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform float PickingID;

void main(){
	// color = vs_vertexColor;
	color = vec4(PickingID/255.0, 0.0, 0.0, 1.0);
}