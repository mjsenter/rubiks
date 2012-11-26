#include "rubiksCube.h"


rubiksCube::rubiksCube( int dimensions ) {

	colors = (vec4 *)malloc( sizeof( vec4 ) * 6 );
	colors[0] = vec4( 0, 0.85, 0, 0 );
	colors[1] = vec4( 0.0, 0.0, 0.85, 1.0 );
	colors[2] = vec4( 0.7, 0.7, 0.7, 1.0 );
	colors[3] = vec4( 0.85, 0.85, 0.0, 1.0 );
	colors[4] = vec4( 0.85, 0.0, 0.0, 1.0 );
	colors[5] = vec4( 0.85, 0.35, 0.0, 1.0 );

	dim = dimensions;
	cursor = 0;

	//Front face setup
	front = new Side( dimensions, colors[0] );
	front->back = new Side( dimensions, colors[1] );
	front->top = new Side( dimensions, colors[2] );
	front->bottom = new Side( dimensions, colors[3] );
	front->right = new Side( dimensions, colors[4] );
	front->left = new Side( dimensions, colors[5] );
	
	//Back face setup
	front->back->back = front;
	front->back->top = front->bottom;
	front->back->right = front->right;
	front->back->left = front->left;
	front->back->bottom = front->top;

	//Top face setup
	front->top->back = front->bottom;
	front->top->top = front->back;
	front->top->right = front->right;
	front->top->left = front->left;
	front->top->bottom = front;

	//Bottom face setup
	front->bottom->back = front->top;
	front->bottom->top = front;
	front->bottom->right = front->right;
	front->bottom->left = front->left;
	front->bottom->bottom = front->back;

	//Right face setup
	front->right->back = front->left;
	front->right->top = front->top;
	front->right->right = front->back;
	front->right->left = front;
	front->right->bottom = front->bottom;

	//Left face setup
	front->left->back = front->right;
	front->left->top = front->bottom;
	front->left->right = front->back;
	front->left->left = front;
	front->left->bottom = front->top;

	Cube cube;
	baseCube = new VertexArray();
	baseCube->AddAttribute( "vPosition", cube.getVertices(), cube.getNumVertices() );
	baseShader = new Shader( "vshader.glsl", "fshader.glsl" );

	face = new VertexArray();
	vec4 facePoints[] = {
		vec3( -0.5, -0.5,  0.0),
		vec3( -0.5,  0.5,  0.0),
		vec3(  0.5,  0.5,  0.0),
		vec3(  0.5, -0.5,  0.0) };
	face->AddAttribute( "vPosition", facePoints, 4 );
	faceShader = new Shader( "vfaceShader.glsl", "ffaceShader.glsl" );

}

rubiksCube::Side::Side( int dim, vec4 color ) {
	homeColor = color;
	colors = (vec4 *)malloc( sizeof( vec4 ) * dim * dim );
	for( int i = 0; i < dim * dim; i++ ) {
		colors[i] = color;
	}
}

void rubiksCube::displayCube( const mat4 & view, const mat4 & proj ) {
	//Display baseCube
	baseShader->Bind();
	baseShader->SetUniform( "view", view );
	baseShader->SetUniform( "model", Scale(1.0) );
	baseShader->SetUniform( "projection", proj );
	baseShader->SetUniform( "color", vec4( 0.0, 0.0, 0.0, 1.0 ) );
	baseCube->Bind( *baseShader );
	baseCube->Draw( GL_TRIANGLES );
	baseCube->Unbind();
	baseShader->Unbind();

	faceShader->Bind();
	face->Bind( *faceShader );
	drawFace( view, proj, 0, front, true );
	drawFace( view, proj, 1, front->back, false );
	drawFace( view, proj, 2, front->top, false );
	drawFace( view, proj, 3, front->bottom, false );
	drawFace( view, proj, 4, front->right, false );
	drawFace( view, proj, 5, front->left, false );
	face->Unbind();
	faceShader->Unbind();
}

void rubiksCube::drawFace( mat4 view, mat4 proj, int rot, Side * side, bool drawCursor ) {
	mat4 scale = Scale( 1 / (GLfloat)dim * 0.9 );
	for( int i = 0; i < dim; i++ ) {
		for( int j = 0; j < dim; j++ ) {
			vec4 col = side->colors[i*dim + j];
			faceShader->SetUniform( "color", side->colors[i*dim + j] );
			mat4 model = Translate( (GLfloat)1/(2*dim) + 
					(GLfloat)j/dim, -(GLfloat)1/(2*dim) - 
					(GLfloat)i/dim, 0.0 ) * 
				Translate( -0.5, 0.5, 0.51 ) * 
				scale;
			switch( rot ) {
				case 1:
					model = RotateY( 180 ) * model;
					break;
				case 2:
					model = RotateX( -90 ) * model;
					break;
				case 3:
					model = RotateX( 90 ) * model;
					break;
				case 4:
					model = RotateY( 90 ) * model;
					break;
				case 5:
					model = RotateY( -90 ) * model;
					break;
			}
			faceShader->SetUniform( "model", model );
			faceShader->SetUniform( "cursor", cursor == i*dim + j && drawCursor);
			faceShader->SetUniform( "view", view );
			faceShader->SetUniform( "projection", proj );
			face->Draw( GL_TRIANGLE_FAN );
		}
	}
}

void rubiksCube::rotate(int index,bool v, bool d) {
	int column;
	int row;
	Side *tempFront = front;
	column = index % dim;
	row = index / dim;
	
	//Vertical
	if(v){
		//rotate up
		if(d){
			for(int i = 0; i < dim; i++){
				front->colors[column + i*dim] = front->bottom->colors[column + i*dim];
				front->bottom->colors[column + i*dim]  = front->back->colors[column + i*dim];
				front->back->colors[column + i*dim] = front->top->colors[column + i*dim];
				front->top->colors[column + i*dim] = tempFront->colors[column + i*dim];
			}
		}
		//rotate down
		if(!d){
			for(int i = 0; i < dim; i++){
				front->colors[column + i*dim] = front->top->colors[column + i*dim];
				front->top->colors[column + i*dim] = front->back->colors[column + i*dim];
				front->back->colors[column + i*dim] = front->bottom->colors[column + i*dim];
				front->bottom->colors[column + i*dim] = tempFront->colors[column + i*dim];
			}
		}
	}
	
	//Horizontal
	if(!v) {
		//rotate right
		if(d) {
			for(int i = 0; i < dim; i++) {
				front->colors[row + i] = front->left->colors[row + i];
				front->left->colors[row + i] = front->back->colors[row + i];
				front->back->colors[row + i] = front->right->colors[row + i];
				front->right->colors[row + i] = tempFront->colors[row + i];
			}
			if(index < dim){
				Side * tempTop = front->top;
				
				for( int i = 0; i < dim; i++ ) {
					for( int j = 0; j < dim; j++ ) {
						front->top->colors[i*dim + j] = tempTop->colors[i + dim * dim - dim * (j + 1 )];
					}
				}
			}
			else if(index > (dim * dim - dim)){
				Side * tempTop = front->top;
				for( int i = 0; i < dim; i++ ) {
					for( int j = 0; j < dim; j++ ) {
						front->top->colors[i*dim + j] = tempTop->colors[dim - i - 1 + j * dim];
					}
				}
			}
		}
		//rotate left
		if(!d){
			for(int i = 0; i < dim; i++){
				front->colors[row + i] = front->right->colors[row + i];
				front->right->colors[row + i] = front->back->colors[row + i];
				front->back->colors[row + i] = front->left->colors[row + i];
				front->left->colors[row + i] = tempFront->colors[row + i];
			}
			if(index < dim){
				Side * tempTop = front->top;
				for( int i = 0; i < dim; i++ ) {
					for( int j = 0; j < dim; j++ ) {
						front->top->colors[i*dim + j] = tempTop->colors[dim - i - 1 + j * dim];
					}
				}
			}
			if(index > (dim * dim - dim)){
			Side * tempTop = front->top;
				for( int i = 0; i < dim; i++ ) {
					for( int j = 0; j < dim; j++ ) {
						front->top->colors[i*dim + j] = tempTop->colors[i + dim * dim - dim * (j + 1 )];
					}
				}	
			}
		}
	}
}

void rubiksCube::rotateCube( int dir ) {
	Side * tempFront = front;
	
	if(dir == 0){
		for(int i = 0; i< dim; i++){
			rotate(i*dim, 0, 1);
		}
		/*
		front = front->left;
		front.left = front.back;
		front.back = front.right;
		front.right = tempfront;
		*/
	}
	if(dir == 1){
		for(int i = 0; i< dim; i++){
			rotate(i*dim, 0, 0);
		}
		/*
		front = front.right;
		front.right = front.back;
		front.back = front.left;
		front.left = tempFront;
		*/
	}
	if(dir == 2){
		for(int i = 0; i< dim; i++){
			rotate(i*dim, 1, 1);
		}
		/*
		front = front.bottom;
		front.bottom = front.back;
		front.back = front.top;
		front.top = tempFront;
		*/
	}
	if(dir == 3){
		for(int i = 0; i< dim; i++){
			rotate(i*dim, 1, 0);
		}
		/*
		front = front.top;
		front.top = front.back;
		front.back = front.bottom;
		front.bottom = tempFront;
		*/
	}
}
void rubiksCube::reset() {}
void rubiksCube::scramble() {}
bool rubiksCube::isWin() {return false;}