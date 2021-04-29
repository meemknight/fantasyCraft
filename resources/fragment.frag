#version 420 core

out vec4 color;


in vec2 v_textureUV;
in float v_ao;

layout(binding = 0) uniform sampler2D u_texture;

void main()
{

	color.rgba = texture2D(u_texture, v_textureUV).rgba;

	float ambiendPart = 0.9;
	float light = (v_ao * ambiendPart) + (1.0 - ambiendPart);

	color.rgb *= light;


} 

