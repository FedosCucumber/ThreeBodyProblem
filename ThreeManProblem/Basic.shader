#shader vertex
#version 330 core

layout(std430, binding = 3) buffer Stuff
{
	float data[160];
};

uniform int u_Index;


void main()
{
	int i = gl_VertexID;
	vec4 position;
	position.x = data[i * 4     + u_Index * 40];
	position.y = data[i * 4 + 1 + u_Index * 40];
	position.z = data[i * 4 + 2 + u_Index * 40];
	position.w = data[i * 4 + 3 + u_Index * 40];

	gl_Position = position;
};
#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
    color = u_Color;
};