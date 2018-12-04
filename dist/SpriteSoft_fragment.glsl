#version 400

in vec2 tex_uv;

layout(location = 0) out vec4 f_colour;

uniform sampler2D u_texture;

void main() {
	vec4 colour = texture2D(u_texture, tex_uv);
	f_colour = colour;
};
