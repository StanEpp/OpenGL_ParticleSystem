#version 430

layout(location=0) in vec4 currentPos;
layout(location=1) in vec4 prevPos;

void main(void){
    gl_Position = currentPos;
}