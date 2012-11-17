#version 150

uniform mat4 view;
uniform mat4 projection;
uniform vec4 color;

in  vec4 vPosition;
out vec4 fColor;

void main() 
{
  fColor = color;
  gl_Position = projection * view * vPosition;
} 
