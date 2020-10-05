#version 330 core

in vec3 modelPos;
out vec3 cubeVertex;
uniform mat4 mvp;

void main() {
	cubeVertex = modelPos;
	gl_Position = mvp * vec4(modelPos, 1.0);
}