//Header file for Rubiks cube project
#ifndef RUBIKSCUBE_H
#define RUBIKSCUBE_H
#include "Angel.h"
#include "Shader.h"
#include "VertexArray.h"
#include "cube.h"

class rubiksCube{
private:
	/*
	 * Class to maintain cube state.
	 */
	class Side {
		public:
			/*
			 * Constructor
			 */
			Side( int dim, vec4 color );

			/*
			 * Destructor
			 */
			~Side();

			vec4 * colors; //Array of colors on the side
			bool * rotate; //Rotate face?
			int numColors; //Number of faces per side
			Side * back;
			Side * right;
			Side * left;
			Side * top;
			Side * bottom;
	};
	Side * front;	//Currently displayed face
	Side * nextFront; //Face to display after animations

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
		int section;	//Which column/row
		int count;		//Current frame in animation
		int numFrames;	//Number of frames for animation
		mat4 transform;
	} Anim;
	Anim * anim;	//Stores animation data

	VertexArray * baseCube;	//VAO for black cube behind colored face
	Shader * baseShader;	//Shader for baseCube
	VertexArray * face;		//Colored face for each individual block
	Shader * faceShader;	//Shader for face

	int cursor;		//Position of cursor on front face
	int dim;		//Dimensions of cube
	unsigned int random;	//Random value used for scramble()

	bool isScrambled;	//Has cube been scrambled?

	float cursorHighlight;	//Highlight amount of cursor
	float inc;	//Incremental change used for cursor highlighting

	/* 
	 * Helper method for displayCube.  Draws one side of the cube. 
	 */
	void drawFace( mat4 view, mat4 proj, int rot, Side * side, bool drawCursor );

	/* 
	 * Helper method for constructor.  Generates one side for cube.
	 * Does not handle linkage between sides.
	 */
	Side * createSides( int dimensions );

public:

	/*
	 * Default constructor
	 */
	rubiksCube(){
		rubiksCube(3);
	}

	/* 
	 * Parameterized constructor
	 */
	rubiksCube(int numberOfSides);

	/*
	 * Destructor
	 */
	~rubiksCube();	

	/*
	 * Displays the Rubik's cube.  Should be called in the Display() function
	 */
	void displayCube( const mat4 & view, const mat4 & proj );

	/*
	 * Rotates a row up/down or a column right/left from cursor position.
	 * v = true: rotate a column;  v = false: rotate a row
	 * d = true: rotate up/right;  d = false: rotate down/left
	 */
	void rotate(bool v, bool d);

	/*
	 * Rotates entier cube along X or Y axis
	 * v = true: rotate vertically;  v = false: rotate horizontally
	 * d = true: rotate up/right;  d = false: rotate down/left
	 */
	void rotateCube( bool v, bool d );

	/*
	 * Randomly rotates a row and column.
	 */
	void scramble();

	/*
	 * Returns whether cube is solved.  Must be scrambled first.
	 */
	bool isWin();

	/*
	 * Updates cursor and animations for each frame.
	 * Called for each frame.
	 */
	void update();

	/*
	 * Moves cursor one place to right on front face.  
	 * Returns true if cursor moved.  Returns false if it
	 * can't move any further.
	 */
	bool moveCursorRight();

	/*
	 * Moves cursor one place to left on front face.  
	 * Returns true if cursor moved.  Returns false if it
	 * can't move any further.
	 */
	bool moveCursorLeft();

	/*
	 * Moves cursor one place up on front face.  
	 * Returns true if cursor moved.  Returns false if it
	 * can't move any further.
	 */
	bool moveCursorUp();

	/*
	 * Moves cursor one place down on front face.  
	 * Returns true if cursor moved.  Returns false if it
	 * can't move any further.
	 */
	bool moveCursorDown();

	/*
	 * Returns number of blocks in a row/column.
	 */
	int getDimensions();

	/*
	 * Returns position of cursor.  
	 */
	int getCursor();
};
#endif