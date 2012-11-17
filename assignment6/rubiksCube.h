//Header file for Rubiks cube project
#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H
#include "Angel.h"
#include "Shader.h"
#include "VertexArray.h"

class Cube{
private:

	struct piece{
		struct face *faces[3];
		mat4 transform;
	};

	struct face{
		vec4 color;
		vec3 points[4];
	};
/*Cube*/
	vec4 cubeVertices[8] = {
	  vec4( -0.5, -0.5,  0.5, 1.0 ),
	  vec4( -0.5,  0.5,  0.5, 1.0 ),
	  vec4(  0.5,  0.5,  0.5, 1.0 ),
	  vec4(  0.5, -0.5,  0.5, 1.0 ),
	  vec4( -0.5, -0.5, -0.5, 1.0 ),
	  vec4( -0.5,  0.5, -0.5, 1.0 ),
	  vec4(  0.5,  0.5, -0.5, 1.0 ),
	  vec4(  0.5, -0.5, -0.5, 1.0 )
	};

	vec4 cubeColor = {0,0,0,1.0};

public:
	Cube(){
		Cube(3);
	}
	Cube(int numberOfSides);
	~Cube();
	//Index of array, vertical or horizontal, up or down
	void rotate(int index,bool v, bool d);
	void reset();
	void scramble();

}
#endif