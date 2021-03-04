#version 420 core

out vec4 color;

in vec2 v_textureUV;

layout(binding = 0) uniform sampler2D u_texture;

void main()
{

	color = vec4(170/255.f, 80/255.f, 173/255.f, 1);
	color.rgb = texture2D(u_texture, v_textureUV).rgb;

} 

