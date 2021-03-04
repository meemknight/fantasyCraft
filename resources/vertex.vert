#version 330 core

in layout(location = 0) vec3 pos;

void main()
{

	gl_Position = vec4(pos, 1);

} 
