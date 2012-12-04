#version 150

uniform bool cursor;
uniform vec4 highlight;

in  vec4 fColor;
out vec4 color;

void main() 
{ 
    color = fColor;
	if( cursor ) {
		color += highlight;
	}
} 

