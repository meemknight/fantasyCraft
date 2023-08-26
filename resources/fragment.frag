#version 420 core

out vec4 color;


in vec2 v_textureUV;
in float v_ao;

layout(binding = 0) uniform sampler2D u_texture;
uniform int u_ao;

void main()
{

	color.rgba = texture2D(u_texture, v_textureUV).rgba;

	float ambiendPart = 0.6;
	if(u_ao == 0){ambiendPart = 0;}

	float light = (v_ao * v_ao * ambiendPart) + (1.0 - ambiendPart);

	color.rgb *= light;


} 

