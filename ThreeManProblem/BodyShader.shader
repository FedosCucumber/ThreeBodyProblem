#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(std430, binding = 3) buffer Stuff
{
	vec4 data[40];
};

uniform mat4 u_MVP;
uniform int u_Index;


void main()
{
	vec4 end = u_MVP * position;
	for (int i = 9; i > 0; --i)
	{
		data[i + u_Index * 10] = data[i - 1 + u_Index * 10];
	}
	data[u_Index * 10] = end;
	gl_Position = end;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
	color = u_Color;
};