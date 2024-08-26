#version 400

in vec2 tex_uv;

out vec4 f_colour;

uniform sampler2D u_texture;

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
	f_colour = colour;
};
