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
int animFrame = 1;
int numAnimFrames = 15;
int rotDir = 0;
bool rotating = false;
int randomNumber;

VertexArray * face;
Shader * faceShader;

struct faceNode {
	mat4 transform;
	vec4 color;
};

struct faceNode * faces;

struct faceNode * tempFaces;

// faceNode used to compare for an isWin state
struct faceNode * solvedFaces;

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

// elapsed time
int elapsedTime;

// frame rate in millis for 30 frames/sec
const int frameRate = 1000.0 / 30;

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
		vec3( -0.5,  0.5,  0.0),
		vec3(  0.5,  0.5,  0.0),
		vec3(  0.5, -0.5,  0.0) };
	face->AddAttribute( "vPosition", facePoints, 4 );
	faceShader = new Shader( "vfaceShader.glsl", "ffaceShader.glsl" );

	faces = new struct faceNode[dim * dim * 6];
	solvedFaces = new struct faceNode[dim * dim * 6];
	tempFaces = new struct faceNode[dim * dim * 6];

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
	for(int i = 0; i < dim * dim * 6; i++){
		solvedFaces[i] = faces[i];
		tempFaces[i] = faces[i];
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

	//Find orientation of cube
	switch( frontFace ) {
		case 0:	//Green face
			switch( upFace ) {
				case 2:	//White face
					rot = 0;
					break;
				case 3:	//Yellow face
					rot = 2;
					break;
				case 4:	//Red face
					rot = 1;
					break;
				case 5: //Orange face
					rot = 3;
					break;
			}
			break;
		case 1:	//Blue face
			switch( upFace ) {
				case 2:	//White face
					rot = 0;
					break;
				case 3:	//Yellow face
					rot = 2;
					break;
				case 4:	//Red face
					rot = 3;
					break;
				case 5:	//Orange face
					rot = 1;
					break;
			}
			break;
		case 2:	//White face
			switch( upFace ) {
				case 0:	//Green face
					rot = 2;
					break;
				case 1:	//Blue face
					rot = 0;
					break;
				case 4:	//Red face
					rot = 1;
					break;
				case 5: //Orange face
					rot = 3;
					break;
			}
			break;
		case 3:	//Yellow face
			switch( upFace ) {
				case 0:	//Green face
					rot = 2;
					break;
				case 1:	//Blue face
					rot = 0;
					break;
				case 4:	//Red face
					rot = 3;
					break;
				case 5: //Orange face
					rot = 1;
					break;
			}
			break;
		case 4:	//Red face
			switch( upFace ) {
				case 0:	//Green face
					rot = 3;
					break;
				case 1:	//Blue face
					rot = 1;
					break;
				case 2:	//White face
					rot = 0;
					break;
				case 3:	//Yellow face
					rot = 2;
					break;
			}
			break;
		case 5:	//Orange face
			switch( upFace ) {
				case 0:	//Green face
					rot = 3;
					break;
				case 1:	//Blue face
					rot = 1;
					break;
				case 2:	//White face
					rot = 2;
					break;
				case 3: //Yellow face
					rot = 0;
					break;
			}
			break;
	}

	std::cout << "Rot: " << rot << std::endl;

	//Set cursorMap based on orientation of cube
	switch( rot ) {
		case 0:	//Face oriented up
			for( i = 0; i < dim * dim; i++ ) {
				cursorMap[i] = index + i;
			}
			break;
		case 1:	//Face rotated 90 CW
			for( i = 0; i < dim; i++ ) {
				for( j = 0; j < dim; j++ ) {
					cursorMap[i*dim + j] = dim - i - 1 + j * dim + index;
				}
			}
			break;
		case 2:	//Face upside down
			for( i = 0; i < dim * dim; i++ ) {
				cursorMap[i] = dim * dim - 1 - i + index;
			}
			break;
		case 3:	//Face rotated 90 CCW
			for( i = 0; i < dim; i++ ) {
				for( j = 0; j < dim; j++ ) {
					cursorMap[i*dim + j] = i + dim * dim - dim * (j + 1 ) + index;
				}
			}
			break;
	}
	std::cout << "cursor: " << cursor << std::endl;
	std::cout << "cursorMap: " << cursorMap[cursor] << std::endl;
}


/* Colors:
 * 0: Front - Green
 * 1: Back - Blue
 * 2: Top - White
 * 3: Bottom - Yellow
 * 4: Right - Red
 * 5: Left - Orange
 */
void scramble(void){
	srand(time(NULL));
	//randomNumber = rand() % 12;

		randomNumber = rand() % 5;
		for(int j = 3; j < 4; j++){
		switch(j){
			//Left
			case 0:	
				for(int i = 0; i< dim; i++){

					faces[i*dim].transform = 
							 RotateX( 90 ) * faces[i*dim].transform;
					faces[i*dim + 2*dim*dim ].transform = 
							 RotateX( 90 ) * faces[i*dim + 2*dim*dim].transform;
					faces[2*dim*dim - 1 - i*dim].transform = 
							 RotateX( 90 ) * faces[2*dim*dim - 1 - i*dim].transform;
					faces[4*dim*dim - 1 - i*dim].transform = 
							 RotateX( 90 ) * faces[4*dim*dim - 1 - i*dim].transform;
					
					
					
					tempFaces[i*dim] = faces[i*dim];
					tempFaces[i*dim + 2*dim*dim] = faces[i*dim + 2*dim*dim];
					tempFaces[2*dim*dim - 1 - i*dim] = faces[i*dim + 2*dim*dim];
					tempFaces[4*dim*dim - 1 - i*dim] = faces[i*dim + 2*dim*dim];
				}

				for(int i = 0; i < dim*dim; i++){
					faces[5*dim*dim + i].transform = 
							RotateX(90) * faces[5*dim*dim + i].transform;
					tempFaces[5*dim*dim + i] = faces[5*dim*dim + i];
				}
				break;
				//Right
			case 1:
				for(int i = 0; i< dim; i++){

					faces[i*dim + dim - 1].transform = 
							 RotateX( -90 ) * faces[i*dim + dim - 1].transform;
					faces[3*dim*dim - 1 - i*dim].transform = 
							 RotateX( -90 ) * faces[3*dim*dim - 1 - i*dim].transform;
					faces[i*dim + dim*dim].transform = 
							 RotateX( -90 ) * faces[i*dim + dim*dim].transform;
					faces[i*dim + 3*dim*dim].transform = 
							 RotateX( -90 ) * faces[i*dim + 3*dim*dim].transform;

					tempFaces[i*dim + dim - 1] = faces[i*dim + dim - 1];
					tempFaces[3*dim*dim - 1 - i*dim] = faces[3*dim*dim - 1 - i*dim];
					tempFaces[i*dim + dim*dim] = faces[i*dim + dim*dim];
					tempFaces[i*dim + 3*dim*dim] = faces[i*dim + 3*dim*dim];
				}
				for(int i = 0; i < dim*dim; i++){
					faces[4*dim*dim + i].transform = 
							RotateX(-90) * faces[4*dim*dim + i].transform;
					tempFaces[4*dim*dim + i] = faces[4*dim*dim + i];
				}
				break;
			//Front
			case 2:
				for(int i = 0; i< dim; i++){

					faces[i*dim + 4*dim*dim].transform = 
							 RotateZ( -90 ) * faces[i*dim + 4*dim*dim].transform;
					faces[3*dim*dim - i - 1].transform = 
							 RotateZ( -90 ) * faces[3*dim*dim - i - 1].transform;
					faces[5*dim*dim + i*dim].transform = 
							 RotateZ( -90 ) * faces[5*dim*dim + i*dim].transform;
					faces[4*dim*dim - 1 - i].transform = 
							 RotateZ( -90 ) * faces[4*dim*dim - 1 - i].transform;
				}
				for(int i = 0; i < dim*dim; i++){
					faces[i].transform = 
							RotateZ(-90) * faces[i].transform;
				}
				break;
			//Back
			case 3:
				for(int i = 0; i< dim; i++){

					faces[2*dim*dim + i].transform = 
										RotateZ( 90 ) * faces[2*dim*dim + i].transform;
					faces[4*dim*dim + i*dim + dim - 1].transform = 
										RotateZ( 90 ) * faces[4*dim*dim + i*dim + dim - 1].transform;
					faces[5*dim*dim + i*dim + dim - 1].transform = 
										 RotateZ( 90 ) * faces[5*dim*dim + i*dim + dim - 1].transform;
					faces[3*dim*dim + i].transform = 
										 RotateZ( 90 ) * faces[3*dim*dim + i].transform;
				}
				for(int i = 0; i < dim*dim; i++){
					faces[dim*dim + i].transform = 
							RotateZ(90) * faces[dim*dim + i].transform;
				}
				break;
			//Up
			case 4:
				for(int i = 0; i< dim; i++){

					faces[i].transform = 
							 RotateY( -90 ) * faces[i].transform;
					faces[4*dim*dim + i].transform = 
							 RotateY( -90 ) * faces[4*dim*dim + i].transform;
					faces[dim*dim + i].transform = 
							 RotateY( -90 ) * faces[dim*dim + i].transform;
					faces[6*dim*dim - i - 1].transform = 
							 RotateY( -90 ) * faces[6*dim*dim - i - 1].transform;
				}
				for(int i = 0; i < dim*dim; i++){
					faces[2*dim*dim + i].transform = 
							RotateY(-90) * faces[2*dim*dim + i].transform;
				}
				break;
			//Down
			case 5:
				for(int i = 0; i< dim; i++){

					faces[dim*dim - i - 1].transform = 
							 RotateY( 90 ) * faces[dim*dim - i - 1].transform;
					faces[5*dim*dim - i - 1].transform = 
							 RotateY( 90 ) * faces[5*dim*dim - i - 1].transform;
					faces[2*dim*dim - i - 1].transform = 
							 RotateY( 90 ) * faces[2*dim*dim - i - 1].transform;
					faces[5*dim*dim + i ].transform = 
							 RotateY( 90 ) * faces[5*dim*dim + i].transform;
				}
				for(int i = 0; i < dim*dim; i++){
					faces[4*dim*dim - i - 1].transform = 
							RotateY(90) * faces[4*dim*dim - i - 1].transform;
				}

			break;
		}
		/*for(int i = 0; i < 6*dim*dim; i++){
			faces[i] = solvedFaces[i];
			faces[i] = tempFaces[i];
		}*/
		}
}


void keyboard( unsigned char key, int x, int y ) {
	if( !rotating ) { //Dont accept input while rotating
		int temp;
		switch( key ) {
			case 033:
			case 'q': 
			case 'Q':
				exit( EXIT_SUCCESS );
				break; 
			case 'w':
				rotating = true;
				rotDir = 0;
				temp = frontFace;
				frontFace = upFace;
				upFace = temp + ( temp % 2 ? -1 : 1 );
				break;
			case 'a':
				rotating = true;
				rotDir = 2;
				temp = rightFace;
				rightFace = frontFace;
				frontFace = temp + ( temp % 2 ? -1 : 1 );
				break;
			case 'd':  
				rotating = true;
				rotDir = 3;
				temp = frontFace;
				frontFace = rightFace;
				rightFace = temp + ( temp % 2 ? -1 : 1 );
				break;
			case 's':
				rotating = true;
				rotDir = 1;
				temp = upFace;
				upFace = frontFace;
				frontFace = temp + ( temp % 2 ? -1 : 1 );
				break;

			case 'r':
				scramble();
				break;
		}
	
		std::cout << "front: " << frontFace << std::endl << "up: " << upFace << std::endl << "right: " << rightFace << std::endl << std::endl;
		glutPostRedisplay();
	}
}

void keyboardSpecial( int key, int x, int y ) {
	if( !rotating ) {	//Don't accept input while rotating
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
	}
	glutPostRedisplay();
}


/* isWin() is a boolean that checks to see if the cube is solved.
 * It checks the front face to see if all of the faces are the same color, then checks the up, then right. 
 */
bool isWin() {
	//TODO ALEX
	int i;
	int flag = 0;
	//Checks to see if 1 face is completed
	for(i = 0; i< dim * dim; i++){
		if(solvedFaces[i].color !=  faces[i].color){
			flag = 1;
			return false;
		}
	}
	
	//Checks to see if the next 3 are completed, if so, the cube is considered to be complete
	if(flag == 0){
		for(i = dim * dim; i < dim * dim * 3; i++){
			if(solvedFaces[i].color != faces[i].color){
				flag = 1;
				return false;
			}
		}
	}
	//cube is completed, returns true
	if(flag == 0){
		return true;
	}

	std::cout<<"Front Face: "<< frontFace<<std::endl;
	return false;
}



void idle( void )
{
  int now = glutGet(GLUT_ELAPSED_TIME);
  if (now - elapsedTime > frameRate)
  {
    elapsedTime = now;
	if( rotating ) {
		switch( rotDir ) {
			case 0: 
				model = RotateX( 1 / (float)numAnimFrames * 90 ) * model;
				break;
			case 1:
				model = RotateX( 1 / (float)numAnimFrames * -90 ) * model;
				break;
			case 2:
				model = RotateY( 1 / (float)numAnimFrames * 90 ) * model;
				break;
			case 3:
				model = RotateY( 1 / (float)numAnimFrames * -90 ) * model;
				break;
		}
		animFrame++;
		if( animFrame > numAnimFrames ) {
			animFrame = 1;
			calcCursorMap();
			rotating = false;
		}
	}
    glutPostRedisplay();
  }
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
	glutIdleFunc( idle );
	glutMainLoop();
	return 0;
}