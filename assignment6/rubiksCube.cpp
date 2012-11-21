/* TODO
Alex:
Randomizer, isWin, rotations

Mike:
DONE: Shaders, Init Transforms, Cube rotations
I plan on fixing the cursor.  Feel free to fix it yourself, though.
Depending on how long this takes to fix I'll work on animating cube rotations.

Keyboard controls
*/



#include "Angel.h"
#include "Shader.h"
#include "VertexArray.h"
#include "cube.h"
#include "Camera.h" 

Camera * camera = new Camera( vec3( 0.9, 0.9, 2.0 ) );

VertexArray * baseCube;
Shader * baseShader;
mat4 model;

VertexArray * face;
Shader * faceShader;

struct faceNode {
	mat4 transform;
	vec4 color;
};

struct faceNode * faces;

/* Colors:
 * 0: Front - Green
 * 1: Back - Blue
 * 2: Top - White
 * 3: Bottom - Yellow
 * 4: Right - Red
 * 5: Left - Orange
 */
vec4 colors[6] = {
	vec4( 0, 0.85, 0, 0 ),
	vec4( 0.0, 0.0, 0.85, 1.0 ),
	vec4( 0.7, 0.7, 0.7, 1.0 ),	
	vec4( 0.85, 0.85, 0.0, 1.0 ),
	vec4( 0.85, 0.0, 0.0, 1.0 ),
	vec4( 0.85, 0.35, 0.0, 1.0 ) };

int dim;

int cursor;	//Index of cursor
int * cursorMap;
int frontFace;
int upFace;
int rightFace;

int xRot = 0;
int yRot = 0;
int zRot = 0;


void init( int dimensions ) {
	dim = dimensions;

	camera->LookLeft( 25 );
	camera->LookDown( 25 );
	camera->MoveForward( 1.5 );

	cursor = 0;
	cursorMap = new int[dim * dim];
	for(int i = 0; i < dim * dim; i++ ) {
		cursorMap[i] = i;
	}
	frontFace = 0;
	upFace = 2;
	rightFace = 4;

	Cube cube;
	baseCube = new VertexArray();
	baseCube->AddAttribute( "vPosition", cube.getVertices(), cube.getNumVertices() );
	baseShader = new Shader( "vshader.glsl", "fshader.glsl" );
	model = Scale( 1.0 );


	face = new VertexArray();
	vec4 facePoints[] = {
		vec3( -0.5, -0.5,  0.0),
		vec3( 0.0,  0.5,  0.0),
		//vec3(  0.5,  0.5,  0.0),
		vec3(  0.5, -0.5,  0.0) };
	face->AddAttribute( "vPosition", facePoints, 3 );
	faceShader = new Shader( "vfaceShader.glsl", "ffaceShader.glsl" );

	faces = new struct faceNode[dim * dim * 6];
	mat4 scale = Scale( 1 / (GLfloat)dim * 0.9 );
	for( int i = 0; i < 6; i++ ) {
		for( int j = 0; j < dim; j++ ) {
			for( int k = 0; k < dim; k++ ) {
				faces[i*dim*dim + j*dim + k].transform = 
					Translate( (GLfloat)1/(2*dim) + (GLfloat)k/dim, -(GLfloat)1/(2*dim) - (GLfloat)j/dim, 0.0 ) * 
					Translate( -0.5, 0.5, 0.51 ) * 
					scale;
				faces[i*dim*dim + j*dim + k].color = colors[i];
				switch( i ) {
					case 1:
						faces[i*dim*dim + j*dim + k].transform = 
							RotateZ( 180 ) * RotateX( 180 ) * faces[i*dim*dim + j*dim + k].transform;
						break;
					case 2:
						faces[i*dim*dim + j*dim + k].transform = 
							RotateX( -90 ) * faces[i*dim*dim + j*dim + k].transform;
						break;
					case 3:
						faces[i*dim*dim + j*dim + k].transform = 
							RotateY( 180 ) * RotateX( 90 ) * faces[i*dim*dim + j*dim + k].transform;
						break;
					case 4:
						faces[i*dim*dim + j*dim + k].transform = 
							RotateY( 90 ) * faces[i*dim*dim + j*dim + k].transform;
						break;
					case 5:
						faces[i*dim*dim + j*dim + k].transform = 
							RotateX( 180 ) * RotateY( -90 ) * faces[i*dim*dim + j*dim + k].transform;
						break;
				}
			}
		}	
	}


	glEnable( GL_DEPTH_TEST );
	glClearColor( .25, .25, .25, 1.0 );
}

void display() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//Draw baseCube
	baseShader->Bind();
	baseShader->SetUniform( "model", model );
	baseShader->SetUniform( "view", camera->GetView() );
	baseShader->SetUniform( "projection", camera->GetProjection() );
	baseShader->SetUniform( "color", vec4( 0.0, 0.0, 0.0, 1.0 ) );
	baseCube->Bind( *baseShader );
	baseCube->Draw( GL_TRIANGLES );
	baseCube->Unbind();
	baseShader->Unbind();

	//Draw faces
	faceShader->Bind();
	face->Bind( *faceShader );
	for( int i = 0; i < dim*dim*6; i++ ) {
		faceShader->SetUniform( "color", faces[i].color );
		faceShader->SetUniform( "model", model * faces[i].transform );
		faceShader->SetUniform( "cursor", cursorMap[cursor] == i );
		faceShader->SetUniform( "view", camera->GetView() );
		faceShader->SetUniform( "projection", camera->GetProjection() );
		face->Draw( GL_TRIANGLE_FAN );
	}
	face->Unbind();
	faceShader->Unbind();

	glutSwapBuffers();
}

void calcCursorMap() {
	int i, j;
	int rot = 0;
	int index = frontFace * dim * dim;
	std::cout << "Index: " << index << std::endl;
	switch( frontFace ) {
		case 0:
			switch( upFace ) {
				case 2:
					rot = 0;
					break;
				case 3:
					rot = 2;
					break;
				case 4:
					rot = 1;
					break;
				case 5: 
					rot = 3;
					break;
			}
			break;
		case 1:
			switch( upFace ) {
				case 2:
					rot = 0;
					break;
				case 3:
					rot = 2;
					break;
				case 4:
					rot = 3;
					break;
				case 5: 
					rot = 1;
					break;
			}
			break;
		case 2:
			switch( upFace ) {
				case 0:
					rot = 2;
					break;
				case 1:
					rot = 0;
					break;
				case 4:
					rot = 1;
					break;
				case 5: 
					rot = 3;
					break;
			}
			break;
		case 3:
			switch( upFace ) {
				case 0:
					rot = 2;
					break;
				case 1:
					rot = 0;
					break;
				case 4:
					rot = 3;
					break;
				case 5: 
					rot = 1;
					break;
			}
			break;
		case 4:
			switch( upFace ) {
				case 0:
					rot = 3;
					break;
				case 1:
					rot = 1;
					break;
				case 2:
					rot = 0;
					break;
				case 3: 
					rot = 2;
					break;
			}
			break;
		case 5:
			switch( upFace ) {
				case 0:
					rot = 3;
					break;
				case 1:
					rot = 1;
					break;
				case 2:
					rot = 2;
					break;
				case 3: 
					rot = 0;
					break;
			}
			break;
	}

	std::cout << "Rot: " << rot << std::endl;

	switch( rot ) {
		case 0:
			for( i = 0; i < dim * dim; i++ ) {
				cursorMap[i] = index + i;
			}
			break;
		case 1:
			for( i = 0; i < dim; i++ ) {
				for( j = 0; j < dim; j++ ) {
					cursorMap[i*dim + j] = dim - i - 1 + j * dim + index;
				}
			}
			break;
		case 2:
			for( i = 0; i < dim * dim; i++ ) {
				cursorMap[i] = dim * dim - 1 - i + index;
			}
			break;
		case 3:
			for( i = 0; i < dim; i++ ) {
				for( j = 0; j < dim; j++ ) {
					cursorMap[i*dim + j] = i + dim * dim - dim * (j + 1 ) + index;
				}
			}
			break;
	}
	//cursorMap[0] = index;
	std::cout << "cursor: " << cursor << std::endl;
	std::cout << "cursorMap: " << cursorMap[cursor] << std::endl;
}

void keyboard( unsigned char key, int x, int y ) {
	int temp;
	switch( key ) {
		case 033:
		case 'q': 
		case 'Q':
			exit( EXIT_SUCCESS );
			break; 
		case 'w':
			model = RotateX( 90 ) * model;
			temp = frontFace;
			frontFace = upFace;
			upFace = temp + ( temp % 2 ? -1 : 1 );
			break;
		case 'a':
			model = RotateY( 90 ) * model;
			temp = rightFace;
			rightFace = frontFace;
			frontFace = temp + ( temp % 2 ? -1 : 1 );
			break;
		case 'd':  
			model = RotateY( -90 ) * model;
			temp = frontFace;
			frontFace = rightFace;
			rightFace = temp + ( temp % 2 ? -1 : 1 );
			break;
		case 's':
			model = RotateX( -90 ) * model;
			temp = upFace;
			upFace = frontFace;
			frontFace = temp + ( temp % 2 ? -1 : 1 );
			break;
	}
	calcCursorMap();
	std::cout << "front: " << frontFace << std::endl << "up: " << upFace << std::endl << "right: " << rightFace << std::endl << std::endl;
	glutPostRedisplay();
}

void keyboardSpecial( int key, int x, int y ) {
	switch( key ) {
		case GLUT_KEY_UP:
			if( cursor >= dim ) {
				cursor -= dim;
			}
			break;
		case GLUT_KEY_DOWN:
			if( cursor + dim < dim * dim ) {
				cursor += dim;
			}
			break;
		case GLUT_KEY_RIGHT:
			if( cursor % dim != dim - 1 ) {
				cursor++;
			}
			break;
		case GLUT_KEY_LEFT:
			if( cursor % dim != 0 ) {
				cursor--;
			}
			break;
	}
	glutPostRedisplay();
}

bool isWin() {
	//TODO ALEX
	return false;
}

int main( int argc, char **argv )
{
  glutInit( &argc, argv );

  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

  glutInitWindowSize( 512, 512 );
  glutCreateWindow( " " );

  glewInit();

  init( 3 );

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(keyboardSpecial);

  glutMainLoop();
  return 0;
}