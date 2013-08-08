#version 430

in vec2 ex_TexCoor;
out vec4 color;

uniform sampler2D texture;
uniform float time;

void main(void){
	vec4 output = texture2D(texture, ex_TexCoor)*8*abs(sin(radians(time)));

	output.x = output.x -0.5 + cos(time*0.4+1.5)*0.5;
	output.y = output.y -0.5 + cos(time*0.6)*sin(time*0.3)*0.35;
	output.z = output.z -0.5 + sin(time*0.2)*0.5;
	
	color = output;
}