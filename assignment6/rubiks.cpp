#include "rubiksCube.h"
#include "Camera.h" 


Camera * camera = new Camera( vec3( 0.9, 0.9, 2.0 ) );

rubiksCube * cube;


void init( int dimensions ) {
	camera->LookLeft( 25 );
	camera->LookDown( 25 );
	camera->MoveForward( 1.5 );
		
	cube = new rubiksCube(3);

	glEnable( GL_DEPTH_TEST );
	glClearColor( .25, .25, 1.0, 1.0 );
}

void display() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	cube->displayCube( camera->GetView(), camera->GetProjection() );

	glutSwapBuffers();
}


void keyboard( unsigned char key, int x, int y ) {
	//if( !rotating ) { //Dont accept input while rotating
		int temp;
		switch( key ) {
			case 033:
			case 'q': 
			case 'Q':
				exit( EXIT_SUCCESS );
				break; 
			case 'w':
				cube->rotateCube( 2 );
				break;
			case 'a':
				cube->rotateCube( 0 );
				break;
			case 'd':  
				cube->rotateCube( 1 );
				break;
			case 's':
				cube->rotateCube( 3 );
				break;

		//	case 'r':
		//		//scramble();
		//		break;
		//}
		
	}
	glutPostRedisplay();
}

void keyboardSpecial( int key, int x, int y ) {
	//if( !rotating ) {	//Don't accept input while rotating
	//	switch( key ) {
	//		case GLUT_KEY_UP:
	//			if( cursor >= dim ) {
	//				cursor -= dim;
	//			}
	//			break;
	//		case GLUT_KEY_DOWN:
	//			if( cursor + dim < dim * dim ) {
	//				cursor += dim;
	//			}
	//			break;
	//		case GLUT_KEY_RIGHT:
	//			if( cursor % dim != dim - 1 ) {
	//				cursor++;
	//			}
	//			break;
	//		case GLUT_KEY_LEFT:
	//			if( cursor % dim != 0 ) {
	//				cursor--;
	//			}
	//			break;
	//	}
	//}
	//glutPostRedisplay();
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
	//glutIdleFunc( idle );
	glutMainLoop();
	return 0;
}