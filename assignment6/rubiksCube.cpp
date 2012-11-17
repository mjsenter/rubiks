/* TODO
Alex:
Randomizer, isWin, rotations

Mike:
Shaders, Init Transforms

Keyboard controls
*/



#include "Angel.h"
#include "Shader.h"
#include "VertexArray.h"
#include "cube.h"
#include "Camera.h" 

Camera * camera = new Camera( vec3( 0.5, 0.5, 2.0 ) );

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

vec4 colors[6];

int numFaces;

int cursor;	//Index of cursor


void init( int dim ) {
	camera->LookLeft( 15 );
	camera->LookDown( 15 );
	camera->MoveForward( 1 );

	cursor = 0;

	Cube cube;
	baseCube = new VertexArray();
	baseCube->AddAttribute( "vPosition", cube.getVertices(), cube.getNumVertices() );
	baseShader = new Shader( "vshader.glsl", "fshader.glsl" );
	model = Scale( 1.0 );


	//face = new VertexArray();
	//vec4 facePoints[] = {
	//	vec3( -0.5, -0.5,  0.5),
	//	vec3( -0.5,  0.5,  0.5),
	//	vec3(  0.5,  0.5,  0.5),
	//	vec3(  0.5, -0.5,  0.5) };
	//face->AddAttribute( "vPosition", facePoints, 4 );
	//faceShader = new Shader( "vfaceShader.glsl", "ffaceShader.glsl" );

	//numFaces = dim * dim * 6;
	//faces = (struct faceNode *)malloc( sizeof( struct faceNode ) * numFaces );
	//TODO Transformation and color settings


	glEnable( GL_DEPTH_TEST );
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
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
	//faceShader->Bind();
	//face->Bind( *faceShader );
	//for( int i = 0; i < numFaces; i++ ) {
	//	faceShader->SetUniform( "color", faces[i].color );
	//	faceShader->SetUniform( "model", faces[i].transform * model );
	//	faceShader->SetUniform( "cursor", cursor == i );
	//	faceShader->SetUniform( "view", camera->GetView() );
	//	faceShader->SetUniform( "projection", camera->GetProjection() );
	//	face->Draw( GL_TRIANGLES );
	//}
	//face->Unbind();
	//faceShader->Unbind();

	glutSwapBuffers();
}

void keyboard( unsigned char key, int x, int y ) {

}

void keyboardSpecial( int key, int x, int y ) {

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