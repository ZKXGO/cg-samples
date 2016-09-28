#version 330 core

in vec3 modelPos;
uniform mat4 mvp;

void main() {
	gl_Position = vec4(modelPos, 1.0) * mvp;
}