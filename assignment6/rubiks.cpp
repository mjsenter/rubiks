#include "rubiksCube.h"
#include "Camera.h" 
#include "TextureCube.h"


Camera * camera = new Camera( vec3( 0.9, 0.9, 2.0 ) );

rubiksCube * cube;

VertexArray * skybox;
Shader * skyShader;
TextureCube * skyboxTexture;
mat4 skyModel;


void init( int dimensions ) {
	camera->LookLeft( 25 );
	camera->LookDown( 25 );
	camera->MoveForward( 1.5 );

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
	//if( !rotating ) { //Dont accept input while rotating
		int temp;
		switch( key ) {
			case 033:
			case 'q': 
			case 'Q':
				exit( EXIT_SUCCESS );
				break; 
			case 'W':
				cube->rotateCube( 2 );
				break;
			case 'A':
				cube->rotateCube( 0 );
				break;
			case 'D':  
				cube->rotateCube( 1 );
				break;
			case 'S':
				cube->rotateCube( 3 );
				break;

			case 's':
				//scramble();
				cube->rotate(true, false);
				cube->isWin();
				break;
			case 'w':
				cube->rotate(true, true);
				cube->isWin();
				break;
			case 'd':
				cube->rotate(false, true);
				cube->isWin();
				break;
			case 'a':
				cube->rotate(false, false);
				cube->isWin();
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
		case GLUT_KEY_UP:
			cube->moveCursorUp();
			break;
		case GLUT_KEY_DOWN:
			cube->moveCursorDown();
			break;
		case GLUT_KEY_RIGHT:
			cube->moveCursorRight();
			break;
		case GLUT_KEY_LEFT:
			cube->moveCursorLeft();
			break;
	}
	glutPostRedisplay();
}

int main( int argc, char **argv )
{
	glutInit( &argc, argv );

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	glutInitWindowSize( 512, 512 );
	glutCreateWindow( " " );

	glewInit();

	init( 4 );

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	//glutIdleFunc( idle );
	glutMainLoop();
	return 0;
}