#version 430

in vec2 ex_TexCoor;
out vec4 color;

uniform sampler2D texture;
uniform float time;

void main(void){
  vec4 outputColor = texture2D(texture, ex_TexCoor)*4;
  
  float red, green, blue;
  
  red   =  cos(time*0.0004+1.5) + 1.f;
  green =  cos(time*0.0006)*sin(time*0.0003)*0.35 + 1.f;
  blue  =  sin(time*0.000002)*0.5 + 1.f;
  
  outputColor.x *=  red;
  outputColor.y *=  green;
  outputColor.z *=  blue;
  
  color = outputColor;
}