#include "Angel.h"
#include "cube.h"
#include "sphere.h"
#include "teapot.h"
#include "Camera.h"
#include "TextureCube.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture2D.h"
#include "ObjFile.h"

#define OBJFILE "../models/cube.obj"
#define OBJTEX "../images/brick.png" 

//Skybox Data
TextureCube * skyboxTexture;
VertexArray * skybox;
Shader * skyShader;
Camera * camera = new Camera();  
mat4 skyModel;

//Object Data
VertexArray * obj;
Texture2D * objTex;
Shader * objShader;
mat4 objModel;

//Stationary light
vec4 sLight( 4.0, 5.0, 0.0, 1.0 );

//Moveable light
vec4 mLight( 0.0, 5.0, 0.0, 1.0 );

void init()
{
	/* Skybox Initialization */
	skyboxTexture = new TextureCube(
	"../images/pos_x.tga",
	"../images/neg_x.tga",
	"../images/pos_y.tga",
	"../images/neg_y.tga",
	"../images/pos_z.tga",
	"../images/neg_z.tga");
	Cube cube;
	skybox = new VertexArray();
	skybox->AddAttribute( "vPosition", cube.getVertices(), cube.getNumVertices() );
	skyModel = Scale( 50.0 );
	skyShader= new Shader( "vshader_cube_tex.glsl", "fshader_cube_tex.glsl" );

	/* Object Initialization */
	ObjFile m( OBJFILE );
	objTex = new Texture2D( OBJTEX );
	obj = new VertexArray();
	obj->AddAttribute( "vPosition", m.GetVertices(), m.GetNumVertices() );
	obj->AddAttribute( "vNormal", m.GetNormals(), m.GetNumVertices() );
	obj->AddAttribute( "vTexCoord", m.GetTexCoords(), m.GetNumVertices() );
	obj->AddIndices( (const unsigned int *) m.GetIndices(), m.GetNumIndices() );
	objModel = Scale( 1.0 );
	objShader = new Shader( "vshader_phong.glsl", "fshader_phong.glsl" );

	glEnable( GL_DEPTH_TEST );
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

//----------------------------------------------------------------------------

void display( void )
{
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

	/* Object Processing */
	mat4 mv = camera->GetView() * objModel;
	mat3 normalMatrix = mat3(vec3(mv[0][0], mv[0][1], mv[0][2]),
                           vec3(mv[1][0], mv[1][1], mv[1][2]),
                           vec3(mv[2][0], mv[2][1], mv[2][2]));
	mat3 light = mat3(
		vec3(0.4, 0.4, 0.4),
		vec3(1.0, 1.0, 1.0),
		vec3(1.0, 1.0, 1.0));
	mat3 material = mat3(
		vec3(1.0, 1.0, 1.0),
		vec3(1.0, 1.0, 1.0),
		vec3(0.99, 0.91, 0.81));
	GLfloat shininess = 28.0;	

	objTex->Bind( 1 );
	objShader->Bind();
	objShader->SetUniform( "model", objModel );
	objShader->SetUniform( "view", camera->GetView() );
	objShader->SetUniform( "projection", camera->GetProjection() );
	objShader->SetUniform( "normalMatrix", normalMatrix );
	objShader->SetUniform( "lightPosition", mLight );
	objShader->SetUniform( "materialProperties", material );
	objShader->SetUniform( "lightProperties", light );
	objShader->SetUniform( "shininess", shininess );
	objShader->SetUniform( "texture", objTex->GetTextureUnit() );
	obj->Bind( *objShader );
	obj->Draw( GL_TRIANGLES );
	obj->Unbind();
	objShader->Unbind();

	glutSwapBuffers();
}

void keyboard( unsigned char key, int x, int y )
{
	switch( key ) {
		case 033: // Escape Key
		case 'q': case 'Q':
			exit( EXIT_SUCCESS );
			break; 
		case 'w':   // forward
			camera->MoveForward(0.5);
			break;
		case 'a':
			camera->MoveLeft(0.5);
			break;
		case 's':   
			camera->MoveBackward(0.5);
			break;
		case 'd':
			camera->MoveRight(0.5);
			break;
		case 'r':
			camera->MoveUp(0.5);
			break;
		case 'f':
			camera->MoveDown(0.5);
			break;
		case 'i':
			camera->LookUp(5);
			break;
		case 'j':
			camera->LookLeft(5);
			break;
		case 'k':
			camera->LookDown(5);
			break;
		case 'l':
			camera->LookRight(5);
			break;
		case 'I':
			camera->PitchUp(5);
			break;
		case 'J':
			camera->HeadLeft(5);
			break;
		case 'K':
			camera->PitchDown(5);
			break;
		case 'L':
			camera->HeadRight(5);
			break;
		case '<':
			camera->RollCCW(5);
			break;
		case '>':
			camera->RollCW(5);
			break;
		case 'o':
			camera->LookAt(vec3(0, 0, 0));
			break;
		case 'u':
			camera->SetFieldOfView(camera->GetFieldOfView() + 5);
			break;
		case 'U':
			camera->SetFieldOfView(camera->GetFieldOfView() - 5);
			break;
	}
	glutPostRedisplay();
}

// Needed to get key events for arrow keys
void keyboardSpecial(int key, int x, int y)
{
	switch( key ) {
		case GLUT_KEY_UP:
			camera->OrbitUp(length(camera->GetPosition()), 5);
			break;
		case GLUT_KEY_DOWN:
			camera->OrbitDown(length(camera->GetPosition()), 5);
			break;
		case GLUT_KEY_RIGHT:
			camera->OrbitRight(length(camera->GetPosition()), 5);
			break;
		case GLUT_KEY_LEFT:
			camera->OrbitLeft(length(camera->GetPosition()), 5);
			break;
	}
	glutPostRedisplay();
}

vec2 mousePos( 0.0, 0.0 );
bool mouseActive = false;

void mouseMotionActive( int x, int y ) {
	if( !mouseActive ) {
		mouseActive = true;
		mousePos = vec2( x, y );
	}
	else {
		mLight.x += ( x - mousePos.x ) / 50;
		mLight.z += ( y - mousePos.y ) / 50;
		mousePos = vec2( x, y );
	}
	glutPostRedisplay();
}

void mouseMotionPassive( int x, int y ) {
	mouseActive = false;
}

int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
	glutInitWindowSize( 512, 512 );
	glutCreateWindow( " " );

	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutMotionFunc(mouseMotionActive);
	glutPassiveMotionFunc(mouseMotionPassive);

	glutMainLoop();
	return 0;
}
