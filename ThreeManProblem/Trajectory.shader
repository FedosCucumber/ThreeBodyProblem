#shader vertex
#version 330 core

layout(std430, binding = 3) buffer Stuff
{
	vec4 data[];
};

uniform int u_Index;


void main()
{
	gl_Position = data[gl_VertexID + 10 * u_Index];
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
    color = u_Color;
};