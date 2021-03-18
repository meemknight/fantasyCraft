#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in ivec3 blockPos;
layout(location = 3) in vec2 atlasUV;

uniform vec3 u_playerPos;
uniform mat4 u_modelView;
uniform mat4 u_projectionMatrix;
uniform ivec3 u_pos;
uniform ivec2 u_atlas;

out vec2 v_textureUV;


void main()
{

	//v_textureUV = (uv + u_atlas) * (1.f/16.f);
	v_textureUV = (uv + atlasUV) * (1.f/16.f);

	vec4 p = vec4(pos, 1);

	//move the block by player position
	p.xyz -= u_playerPos;
	//p.xyz += u_pos;
	p.xyz += blockPos;

	p = u_projectionMatrix * u_modelView * p;

	gl_Position = p;


} 
