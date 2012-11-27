//Header file for Rubiks cube project
#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H
#include "Angel.h"
#include "Shader.h"
#include "VertexArray.h"
#include "cube.h"

class rubiksCube{
private:
	class Side {
		public:
			Side( int dim, vec4 color );
			vec4 homeColor;
			vec4 * colors;
			Side * back;
			Side * right;
			Side * left;
			Side * top;
			Side * bottom;
	};
	Side * front;

	/* Colors:
	 * 0: Front - Green
	 * 1: Back - Blue
	 * 2: Top - White
	 * 3: Bottom - Yellow
	 * 4: Right - Red
	 * 5: Left - Orange
	 */
	 vec4 * colors;

	VertexArray * baseCube;
	Shader * baseShader;
	VertexArray * face;
	Shader * faceShader;


	void drawFace( mat4 view, mat4 proj, int rot, Side * side, bool drawCursor );

public:
	rubiksCube(){
		rubiksCube(3);
	}
	rubiksCube(int numberOfSides);
	//~rubiksCube();
	//Index of array, vertical or horizontal, up or down
	

	void displayCube( const mat4 & view, const mat4 & proj );
	void rotate(int index,bool v, bool d);
	void rotateCube( int dir );
	void reset();
	void scramble();
	bool isWin();

	int cursor;
	int dim;
};
#endif