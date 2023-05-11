#version 330 core
out vec4 color;

in vec3 v_pos;

uniform samplerCube u_cubeMap;

void main()
{

	color = texture(u_cubeMap, v_pos);

}