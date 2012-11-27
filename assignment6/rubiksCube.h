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
			~Side();
			//vec4 homeColor;
			vec4 * colors;
			int numColors;
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

	int cursor;
	int dim;

	void drawFace( mat4 view, mat4 proj, int rot, Side * side, bool drawCursor );

public:
	rubiksCube(){
		rubiksCube(3);
	}
	rubiksCube(int numberOfSides);
	~rubiksCube();	

	void displayCube( const mat4 & view, const mat4 & proj );
	void rotate(int index,bool v, bool d);
	void rotateCube( int dir );
	void reset();
	void scramble();
	bool isWin();

	bool moveCursorRight();
	bool moveCursorLeft();
	bool moveCursorUp();
	bool moveCursorDown();

	int getDimensions();
	int getCursor();
};
#endif