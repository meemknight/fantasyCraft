#version 420 core

out vec4 color;

in vec2 v_textureUV;

layout(binding = 0) uniform sampler2D u_texture;

void main()
{

	color.rgba = texture2D(u_texture, v_textureUV).rgba;

} 

