#version 330 core
layout (location = 0) in vec2 aPos;

uniform float ratio;
uniform mat3 transform;

void main() {
	vec3 prepos = vec3(aPos, 1.0);
	vec4 pos = vec4(transform * prepos, 1.0);

	if (ratio > 1) {
		pos.x /= ratio;
	} else {
		pos.y *= ratio;
	}

	gl_Position = pos;
}