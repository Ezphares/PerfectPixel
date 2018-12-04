#version 400

layout(location = 0) in vec3 xyz;

out vec2 tex_uv;

void main() {
	gl_Position = vec4(xyz, 1.0);
	tex_uv = vec2(clamp(xyz.x, 0, 1), clamp(xyz.y, 0, 1));
};
