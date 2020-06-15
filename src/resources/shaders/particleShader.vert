#version 430 core

uniform mat4 in_Projection;
uniform mat4 in_View;

layout(location = 0) in vec3 in_BillboardVertex;
layout(location = 4) in vec4 in_Position;
layout(location = 6) in vec4 in_Color;

out vec4 ex_Color;

void main()
{
	vec3 CameraRight_worldspace = vec3(in_View[0][0], in_View[1][0], in_View[2][0]);
	vec3 CameraUp_worldspace = vec3(in_View[0][1], in_View[1][1], in_View[2][1]);
	
	vec3 newPosition = 
	vec3 (in_Position.x, in_Position.y, in_Position.z)
	+ (CameraRight_worldspace * in_BillboardVertex.x * 0.1)
	+ (CameraUp_worldspace * in_BillboardVertex.y * 0.1);
	
	gl_Position = in_Projection * in_View * vec4(newPosition, 1.0);

	//gl_Position = in_Projection * in_View * in_Position;
	ex_Color = in_Color;
}