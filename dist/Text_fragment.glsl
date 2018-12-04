#version 400

in vec2 tex_uv;

layout(location = 0) out vec4 f_colour;

uniform sampler2D u_texture;
uniform vec4 u_fontColour;

void main() {
	
	vec4 colour = texture2D(u_texture, tex_uv);
	float brightness = (colour.x + colour.y + colour.z) * 0.33;

	colour = vec4(u_fontColour.x, u_fontColour.y, u_fontColour.z, brightness);
	f_colour = colour;
};
