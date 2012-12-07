#include "rubiksCube.h"
#include "Camera.h" 
#include "TextureCube.h"


Camera * camera = new Camera( vec3( 0.9, 0.9, 2.0 ) );

rubiksCube * cube;

VertexArray * skybox;
Shader * skyShader;
TextureCube * skyboxTexture;
mat4 skyModel;

// elapsed time
int elapsedTime;

// frame rate in millis for 30 frames/sec
const int frameRate = 1000.0 / 30;


void init( int dimensions ) {
	camera->LookLeft( 25 );
	camera->LookDown( 25 );
	camera->MoveForward( 1.5 );

	elapsedTime = 0;

	/* Skybox Initialization */
	skyboxTexture = new TextureCube(
	"../images/pos_x.tga",
	"../images/neg_x.tga",
	"../images/pos_y.tga",
	"../images/neg_y.tga",
	"../images/pos_z.tga",
	"../images/neg_z.tga");
	Cube sky;
	skybox = new VertexArray();
	skybox->AddAttribute( "vPosition", sky.getVertices(), sky.getNumVertices() );
	skyModel = Scale( 5.0 );
	skyShader= new Shader( "vshader_cube_tex.glsl", "fshader_cube_tex.glsl" );
		
	cube = new rubiksCube( dimensions );

	glEnable( GL_DEPTH_TEST );
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

void display() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	/* Skybox processing */
	skyboxTexture->Bind( 1 );
	skyShader->Bind();
	skyShader->SetUniform( "model", skyModel );
	skyShader->SetUniform( "view", camera->GetView() );
	skyShader->SetUniform( "projection", camera->GetProjection() );
	skyShader->SetUniform( "textureCube", skyboxTexture->GetTextureUnit() );
	skybox->Bind( *skyShader );
	skybox->Draw( GL_TRIANGLES );
	skybox->Unbind();
	skyShader->Unbind();

	cube->displayCube( camera->GetView(), camera->GetProjection() );

	glutSwapBuffers();
}


void keyboard( unsigned char key, int x, int y ) {
	int temp;
	switch( key ) {
		case 033:
		case 'q': 
		case 'Q':
			exit( EXIT_SUCCESS );
			break; 

		//Camera Controls
		case 'i':   // forward
			camera->MoveForward(0.5);
			break;
		case 'j':
		camera->LookLeft(5);
		break;
	  case 'k':   
		camera->MoveBackward(0.5);
		break;
	  case 'l':
		camera->LookRight(5);
		break;
	  case 'p':
		camera->MoveUp(0.5);
		break;
	  case ';':
		camera->MoveDown(0.5);
		break;
	  case '9':
		camera->LookUp(5);
		break;
	  case '8':
		camera->LookDown(5);
		break;

	/*Cube Controls*/
		/*Rotate Up*/
		case 'W':
			cube->rotateCube( true, true );
			break;
		/*Rotate left*/
		case 'A':
			cube->rotateCube( false, false );
			break;
		/*Rotate down*/
		case 'D':  
			cube->rotateCube( false, true );
			break;
		/*Rotate right*/
		case 'S':
			cube->rotateCube( true, false );
			break;

		/*Turn row down*/
		case 's':
			cube->rotate(true, false);
			cube->isWin();
			break;
		/*turn row up*/
		case 'w':
			cube->rotate(true, true);
			cube->isWin();
			break;
		/*turn row right*/
		case 'd':
			cube->rotate(false, true);
			cube->isWin();
			break;
		/*turn row left*/
		case 'a':
			cube->rotate(false, false);
			cube->isWin();
			break;

		//scramble
		case 'y':
			cube->scramble();
			break;
		//Reset cube
		case 'z':
		case 'Z':
			int dim = cube->getDimensions();
			delete cube;
			cube = new rubiksCube( dim );
			break;


			
	}
	glutPostRedisplay();
}

void keyboardSpecial( int key, int x, int y ) {
	switch( key ) {
		/*Move cursor up*/
		case GLUT_KEY_UP:
			cube->moveCursorUp();
			break;
			/*Move cursor down*/
		case GLUT_KEY_DOWN:
			cube->moveCursorDown();
			break;
		/*Move cursor right*/
		case GLUT_KEY_RIGHT:
			cube->moveCursorRight();
			break;
		/*Move cursor left*/
		case GLUT_KEY_LEFT:
			cube->moveCursorLeft();
			break;
	}
	glutPostRedisplay();
}

void idle( void )
{
	int now = glutGet(GLUT_ELAPSED_TIME);
	if (now - elapsedTime > frameRate)
	{
		elapsedTime = now;
		cube->update();
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

	/*Where to input the dimensions of the cube*/
	init( 3 );

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutIdleFunc( idle );
	glutMainLoop();
	return 0;
}