#version 430

in vec2 ex_TexCoor;
out vec4 color;

uniform sampler2D texture;
uniform float time;

void main(void){
    vec4 outputColor = texture2D(texture, ex_TexCoor) * 3.f;

    float red, green, blue;

    // Play around with the values to change color of particles over time
    green  = cos(time * 0.2 + 1.5) + 1.f;
    red = cos(time * 0.04) * sin(time * 0.003) * 0.35 + 1.f;
    blue = sin(time * 0.0006) * 0.5 + 1.f;

    outputColor.x *= red;
    outputColor.y *= green;
    outputColor.z *= blue;

    color = outputColor;
}