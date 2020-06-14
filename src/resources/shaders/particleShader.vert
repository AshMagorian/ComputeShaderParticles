#version 430 core

uniform mat4 in_Projection;

layout(location = 4) in vec4 in_Position;
layout(location = 6) in vec4 in_Color;

out vec4 ex_Color;

void main()
{
	gl_Position = in_Projection * in_Position;
	ex_Color = in_Color;
}