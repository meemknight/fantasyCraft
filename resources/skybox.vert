#version 330 core

layout(location = 0) in vec3 pos;

out vec3 v_pos;

uniform mat4 u_viewProjection;

void main()
{

	v_pos = pos;

	vec4 p = u_viewProjection * vec4(pos, 1);

	gl_Position = p.xyww;

}