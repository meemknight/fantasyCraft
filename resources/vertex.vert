#version 330 core

in layout(location = 0) vec3 pos;
in layout(location = 1) vec2 uv;

uniform vec3 u_playerPos;
uniform mat4 u_modelView;
uniform mat4 u_projectionMatrix;

out vec2 v_textureUV;

void main()
{

	v_textureUV = uv;

	vec4 p = vec4(pos, 1);

	//move the block by player position
	p.xyz -= u_playerPos;

	p = u_projectionMatrix * u_modelView * p;

	gl_Position = p;


} 
