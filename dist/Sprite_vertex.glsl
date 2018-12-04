#version 400

layout(location = 0) in vec3 xyz;
layout(location = 1) in vec2 uv;

out vec2 tex_uv;

uniform mat4 u_transform;

void main() {
	gl_Position = u_transform * vec4(xyz, 1.0);
	tex_uv = uv;
};
