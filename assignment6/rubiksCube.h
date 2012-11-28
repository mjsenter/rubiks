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
			VertexArray * cube;
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

	/*
	 * Contains information for animations
	 */
	typedef struct _anim {
		bool rotate;	//Cube is rotating?
		bool vert;		//Vertical = true;  Horizontal = false;
		bool dir;		//Right and Up = true;  Left and Down = false;
		bool cubeRot;	//Rotate entire cube?
		int section;	//Which column/row
		int count;		//Current frame in animation
		int numFrames;	//Number of frames for animation
		mat4 transform;
	} Anim;
	Anim * anim;

	VertexArray * baseCube;
	Shader * baseShader;
	VertexArray * face;
	Shader * faceShader;

	int cursor;
	int dim;

	bool isScrambled;

	void drawFace( mat4 view, mat4 proj, int rot, Side * side, bool drawCursor );

	void sectionRotate( bool v, bool d );
	void cubeRotate( bool v, bool d );

public:
	rubiksCube(){
		rubiksCube(3);
	}
	rubiksCube(int numberOfSides);
	~rubiksCube();	

	void displayCube( const mat4 & view, const mat4 & proj );
	void rotate(bool v, bool d);
	void rotateCube( bool v, bool d );
	void scramble();
	bool isWin();

	void update();

	bool moveCursorRight();
	bool moveCursorLeft();
	bool moveCursorUp();
	bool moveCursorDown();

	int getDimensions();
	int getCursor();
};
#endif