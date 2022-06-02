#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(std430, binding = 3) buffer Stuff
{
	float data[160];
};

uniform mat4 u_MVP;
uniform int u_Index;


void main()
{
	vec4 end = u_MVP * position;
	for (int i = 9; i > 0; --i)
	{
		data[i * 4     + u_Index * 40] = data[(i - 1) * 4     + u_Index * 40];
		data[i * 4 + 1 + u_Index * 40] = data[(i - 1) * 4 + 1 + u_Index * 40];
		data[i * 4 + 2 + u_Index * 40] = data[(i - 1) * 4 + 2 + u_Index * 40];
		data[i * 4 + 3 + u_Index * 40] = data[(i - 1) * 4 + 3 + u_Index * 40];
	}
	data[0 + u_Index * 40] = end.x;
	data[1 + u_Index * 40] = end.y;
	data[2 + u_Index * 40] = end.z;
	data[3 + u_Index * 40] = end.w;

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