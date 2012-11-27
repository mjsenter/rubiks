#version 150

uniform bool cursor;

in  vec4 fColor;
out vec4 color;

void main() 
{ 
    color = fColor;
	if( cursor ) {
		color += vec4( 0.25, 0.25, 0.25, 1.0 );
	}
} 

