#version 400

in vec2 tex_uv;

layout(location = 0) out vec4 f_colour;

uniform sampler2D u_texture;
uniform vec4 u_fontColour;

vec4 sampleAA(sampler2D tex, vec2 uv)
{
    vec2 texsize = vec2(textureSize(tex,0));
    vec2 uv_texspace = uv*texsize;
    vec2 seam = floor(uv_texspace+.5);
    uv_texspace = (uv_texspace-seam)/fwidth(uv_texspace)+seam;
    uv_texspace = clamp(uv_texspace, seam-.5, seam+.5);
    return texture2D(tex, uv_texspace/texsize);
}

void main() {
	
	vec4 colour = sampleAA(u_texture, tex_uv);
	float brightness = (colour.x + colour.y + colour.z) * 0.33;

	colour = vec4(u_fontColour.x, u_fontColour.y, u_fontColour.z, brightness);
	f_colour = colour;
};
