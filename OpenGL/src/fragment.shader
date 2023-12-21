#version 330 core
out vec4 FragColor;
in vec4 gl_FragCoord;

uniform vec4 color;
uniform vec2 resolution;

vec2 pixel = gl_FragCoord.xy;

void main()
{
    FragColor = vec4(color.r, color.g, color.b, color.a);
}