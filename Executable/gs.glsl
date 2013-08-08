#version 430
#extension GL_EXT_geometry_shader4 : enable

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

out vec2 ex_TexCoor;

uniform mat4 	viewMatrix;
uniform mat4 	projMatrix;
uniform vec4 	camPos;
uniform float 	quadLength;

void main(void){

	vec4 normal = gl_PositionIn[0] - camPos;
	normal = viewMatrix*normal;
	
	vec3 rightAxis 	= cross(normal.xyz, vec3(0,1,0));
	vec3 upAxis 	= cross(rightAxis, normal.xyz);
	rightAxis 		= normalize(rightAxis);
	upAxis			= normalize(upAxis);
	
	vec4 rightVector 	= vec4(rightAxis.xyz, 1.0f);
	vec4 upVector 		= vec4(upAxis.xyz, 1.0f);
	vec4 particlePos	= viewMatrix*gl_PositionIn[0];

	gl_Position = particlePos-rightVector*(quadLength*0.5f)-upVector*(quadLength*0.5f);
	gl_Position = projMatrix*gl_Position;
	ex_TexCoor = vec2(0,0);
	EmitVertex();
	
	gl_Position = particlePos+rightVector*(quadLength*0.5f)-upVector*(quadLength*0.5f);
	gl_Position.x += 0.5;
	gl_Position = projMatrix*gl_Position;
	ex_TexCoor = vec2(1,0);
	EmitVertex();
	
	gl_Position = particlePos-rightVector*(quadLength*0.5f)+upVector*(quadLength*0.5f);
	gl_Position.y += 0.5;
	gl_Position = projMatrix*gl_Position;
	ex_TexCoor = vec2(0,1);
	EmitVertex();
	
	gl_Position = particlePos+rightVector*(quadLength*0.5f)+upVector*(quadLength*0.5f);
	gl_Position.y += 0.5;
	gl_Position.x += 0.5;
	gl_Position = projMatrix*gl_Position;
	ex_TexCoor = vec2(1,1);
	EmitVertex();
	
}