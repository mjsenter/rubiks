#include "rubiksCube.h"


rubiksCube::rubiksCube( int dimensions ) {

	colors = (vec4 *)malloc( sizeof( vec4 ) * 6 );
	colors[0] = vec4( 0, 0.85, 0, 0 );
	colors[1] = vec4( 0.0, 0.0, 0.85, 1.0 );
	colors[2] = vec4( 0.7, 0.7, 0.7, 1.0 );
	colors[3] = vec4( 0.85, 0.85, 0.0, 1.0 );
	colors[4] = vec4( 0.85, 0.0, 0.0, 1.0 );
	colors[5] = vec4( 0.85, 0.40, 0.0, 1.0 );

	anim = (Anim *)malloc( sizeof( Anim ) );
	anim->rotate = false;
	anim->count = 0;
	anim->numFrames = 10;

	dim = dimensions;
	cursor = 0;

	isScrambled = false;

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
	numColors = dim * dim;
	colors = (vec4 *)malloc( sizeof( vec4 ) * numColors );
	for( int i = 0; i < numColors; i++ ) {
		colors[i] = color;
	}
}

rubiksCube::~rubiksCube() {
	delete front->back;
	delete front->bottom;
	delete front->top;
	delete front->right;
	delete front->left;
	delete front;
	free( colors );
}

//Side deletion only responsible for itself
//Does not delete links to other sides
rubiksCube::Side::~Side() {
	free( colors );
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

	//Draw faces
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

void rubiksCube::rotate(bool v, bool d) {
	if( anim->rotate ) { //already rotating
		return;
	}
	anim->rotate = true;
	anim->cubeRot = false;
	anim->vert = v;
	anim->dir = d;
	if( v ) {
		anim->section = cursor % dim;
	}
	else {
		anim->section = cursor / dim;
	}
	//int column;
	//int row;
	//Side *tempFront = new Side(dim, vec4(0, 0, 0, 1));
	//for(int i = 0; i< dim*dim; i++){
	//	tempFront->colors[i] = front->colors[i];
	//}
	//column = cursor % dim;
	//row = cursor / dim;
	//
	////Vertical
	//if(v){
	//	//rotate up
	//	if(d){
	//		for(int i = 0; i < dim; i++){
	//			front->colors[column + i*dim] = front->bottom->colors[column + i*dim];
	//			front->bottom->colors[column + i*dim]  = front->back->colors[column + i*dim];
	//			front->back->colors[column + i*dim] = front->top->colors[column + i*dim];
	//			front->top->colors[column + i*dim] = tempFront->colors[column + i*dim];
	//		}
	//	}
	//	//rotate down
	//	if(!d){
	//		for(int i = 0; i < dim; i++){
	//			front->colors[column + i*dim] = front->top->colors[column + i*dim];
	//			front->top->colors[column + i*dim] = front->back->colors[column + i*dim];
	//			front->back->colors[column + i*dim] = front->bottom->colors[column + i*dim];
	//			front->bottom->colors[column + i*dim] = tempFront->colors[column + i*dim];
	//		}
	//	}
	//}
	//
	////Horizontal
	//if(!v) {
	//	//rotate right
	//	if(d) {
	//		for(int i = 0; i < dim; i++) {
	//			front->colors[row*dim + i] = front->left->colors[(dim - row)*dim - i - 1];
	//			front->left->colors[(dim - row)*dim - i - 1] = front->back->colors[(dim - row)*dim - i - 1];
	//			front->back->colors[(dim - row)*dim - i - 1] = front->right->colors[row*dim + i];
	//			front->right->colors[row*dim + i] = tempFront->colors[row*dim + i];
	//		}
	//		
	//		if(row == 0){
	//			Side *tempTop = new Side(dim, vec4(0, 0, 0, 1));
	//				for(int i = 0; i< dim*dim; i++){
	//					tempTop->colors[i] = front->top->colors[i];
	//				}

	//			for( int i = 0; i < dim; i++ ) {
	//				for( int j = 0; j < dim; j++ ) {
	//					front->top->colors[i*dim + j] = tempTop->colors[i + dim * dim - dim * (j + 1 )];
	//				}
	//			}
	//		}
	//		else if(row > (dim - 1)){
	//			Side *tempTop = new Side(dim, vec4(0, 0, 0, 1));
	//				for(int i = 0; i< dim*dim; i++){
	//					tempTop->colors[i] = front->top->colors[i];
	//				}

	//			for( int i = 0; i < dim; i++ ) {
	//				for( int j = 0; j < dim; j++ ) {
	//					front->top->colors[i*dim + j] = tempTop->colors[dim - i - 1 + j * dim];
	//				}
	//			}
	//		}
	//	}
	//	//rotate left
	//	if(!d){
	//		for(int i = 0; i < dim; i++){
	//			front->colors[row*dim + i] = front->right->colors[row*dim + i];
	//			front->right->colors[row*dim + i] = front->back->colors[(dim - row)*dim - i - 1];
	//			front->back->colors[(dim - row)*dim - i - 1] = front->left->colors[(dim - row)*dim - i - 1];
	//			front->left->colors[(dim - row)*dim - i - 1] = tempFront->colors[row*dim + i];
	//		}
	//		if(row == 0){
	//			Side *tempTop = new Side(dim, vec4(0, 0, 0, 1));
	//				for(int i = 0; i< dim*dim; i++){
	//					tempTop->colors[i] = front->top->colors[i];
	//				}

	//			for( int i = 0; i < dim; i++ ) {
	//				for( int j = 0; j < dim; j++ ) {
	//					front->top->colors[i*dim + j] = tempTop->colors[dim - i - 1 + j * dim];
	//				}
	//			}
	//		}
	//		else if(row > (dim -1)){
	//			Side *tempTop = new Side(dim, vec4(0, 0, 0, 1));
	//				for(int i = 0; i< dim*dim; i++){
	//					tempTop->colors[i] = front->top->colors[i];
	//				}
	//				
	//			for( int i = 0; i < dim; i++ ) {
	//				for( int j = 0; j < dim; j++ ) {
	//					front->top->colors[i*dim + j] = tempTop->colors[i + dim * dim - dim * (j + 1 )];
	//				}
	//			}	
	//		}
	//	}
	//}
}

void rubiksCube::rotateCube( bool v, bool d ) {
	if( anim->rotate ) { //Already rotating
		return;
	}
	anim->cubeRot = true;
	anim->rotate = true;
	anim->dir = d;
	anim->vert = v;
}

void rubiksCube::scramble() {}
bool rubiksCube::isWin() {
	if( !isScrambled ) {
		return false;
	}

	//checks front face
	for(int i = 1; i<dim*dim; i++){
		if(front->colors[i].x != front->colors[i - 1].x
			|| front->colors[i].y != front->colors[i - 1].y
			|| front->colors[i].z != front->colors[i - 1].z
			|| front->colors[i].w != front->colors[i - 1].w){
				return false;
		}
	}
	//checks right face
	for(int i = 1; i<dim*dim; i++){
		if(front->right->colors[i].x != front->right->colors[i - 1].x
			|| front->right->colors[i].y != front->right->colors[i - 1].y
			|| front->right->colors[i].z != front->right->colors[i - 1].z
			|| front->right->colors[i].w != front->right->colors[i - 1].w){
				return false;
		}
	}
	//checks top face
	for(int i = 1; i<dim*dim; i++){
		if(front->top->colors[i].x != front->top->colors[i - 1].x
			|| front->top->colors[i].y != front->top->colors[i - 1].y
			|| front->top->colors[i].z != front->top->colors[i - 1].z
			|| front->top->colors[i].w != front->top->colors[i - 1].w){
				return false;
		}
	}
	//checks left face
	for(int i = 1; i<dim*dim; i++){
		if(front->left->colors[i].x != front->left->colors[i - 1].x
			|| front->left->colors[i].y != front->left->colors[i - 1].y
			|| front->left->colors[i].z != front->left->colors[i - 1].z
			|| front->left->colors[i].w != front->left->colors[i - 1].w){
				return false;
		}
	}
	//checks back face
	for(int i = 1; i<dim*dim; i++){
		if(front->back->colors[i].x != front->back->colors[i - 1].x
			|| front->back->colors[i].y != front->back->colors[i - 1].y
			|| front->back->colors[i].z != front->back->colors[i - 1].z
			|| front->back->colors[i].w != front->back->colors[i - 1].w){
				return false;
		}
	}
	std::cout<<"Win!"<<std::endl;
	return true;
}

bool rubiksCube::moveCursorRight() {
	if( cursor % dim != dim - 1 ) {
		cursor++;
		return true;
	}
	return false;
}

bool rubiksCube::moveCursorLeft() {
	if( cursor % dim != 0 ) {
		cursor--;
		return true;
	}
	return false;
}

bool rubiksCube::moveCursorUp() {
	if( cursor >= dim ) {
		cursor -= dim;
		return true;
	}
	return false;
}

bool rubiksCube::moveCursorDown() {
	if( cursor + dim < dim * dim ) {
		cursor += dim;
		return true;
	}
	return false;
}

int rubiksCube::getDimensions() {
	return dim;
}
int rubiksCube::getCursor() {
	return cursor;
}

void rubiksCube::update() {
	if( !anim->rotate ) {
		return;
	}
	
	if( anim->cubeRot ) { //Rotate entire cube
		if( anim->vert ) { //Vertical rotation
			if( anim->dir ) { //Up

			}
			else { //Down

			}
		}
		else { //Horizontal rotation
			if( anim->dir ) { //Right

			}
			else { //Left

			}
		}
	}
	else { //Rotate row or column
		if( anim->vert ) { //Vertical rotation
			if( anim->dir ) { //Up

			}
			else { //Down

			}
		}
		else { //Horizontal rotation
			if( anim->dir ) { //Right

			}
			else { //Left

			}
		}
	}
	anim->count++;
	if( anim->count >= anim->numFrames ) {
		if( anim->cubeRot ) {
			cubeRotate( anim->vert, anim->dir );
		}
		else {
			sectionRotate( anim->vert, anim->dir );
		}
		anim->count = 0;
		anim->rotate = false;
	}
}

void rubiksCube::cubeRotate( bool v, bool d ) {
	Side * tempFront = front;
	int tempCursor = cursor;
	if( v ) {
		if( d ) {
			for(int i = 0; i < dim; i++){
				cursor = i;
				sectionRotate(1, 0);
			}
		}
		else {
			for(int i = 0; i < dim; i++){
				cursor = i;
				sectionRotate(1, 1);
			}
		}
	}
	else {
		if( d ) {
			for(int i = 0; i < dim; i++){
				cursor = i*dim;
				sectionRotate( 0, 0);
			}
		}
		else {
			for(int i = 0; i < dim; i++) {
				cursor = i*dim;
				sectionRotate(0, 1);
			}	
		}
	}
	cursor = tempCursor;
}

void rubiksCube::sectionRotate( bool v, bool d ) {
	int column;
	int row;
	Side *tempFront = new Side(dim, vec4(0, 0, 0, 1));
	for(int i = 0; i< dim*dim; i++){
		tempFront->colors[i] = front->colors[i];
	}
	column = cursor % dim;
	row = cursor / dim;
	
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
				front->colors[row*dim + i] = front->left->colors[(dim - row)*dim - i - 1];
				front->left->colors[(dim - row)*dim - i - 1] = front->back->colors[(dim - row)*dim - i - 1];
				front->back->colors[(dim - row)*dim - i - 1] = front->right->colors[row*dim + i];
				front->right->colors[row*dim + i] = tempFront->colors[row*dim + i];
			}
			
			if(row == 0){
				Side *tempTop = new Side(dim, vec4(0, 0, 0, 1));
					for(int i = 0; i< dim*dim; i++){
						tempTop->colors[i] = front->top->colors[i];
					}

				for( int i = 0; i < dim; i++ ) {
					for( int j = 0; j < dim; j++ ) {
						front->top->colors[i*dim + j] = tempTop->colors[i + dim * dim - dim * (j + 1 )];
					}
				}
			}
			else if(row > (dim - 1)){
				Side *tempTop = new Side(dim, vec4(0, 0, 0, 1));
					for(int i = 0; i< dim*dim; i++){
						tempTop->colors[i] = front->top->colors[i];
					}

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
				front->colors[row*dim + i] = front->right->colors[row*dim + i];
				front->right->colors[row*dim + i] = front->back->colors[(dim - row)*dim - i - 1];
				front->back->colors[(dim - row)*dim - i - 1] = front->left->colors[(dim - row)*dim - i - 1];
				front->left->colors[(dim - row)*dim - i - 1] = tempFront->colors[row*dim + i];
			}
			if(row == 0){
				Side *tempTop = new Side(dim, vec4(0, 0, 0, 1));
					for(int i = 0; i< dim*dim; i++){
						tempTop->colors[i] = front->top->colors[i];
					}

				for( int i = 0; i < dim; i++ ) {
					for( int j = 0; j < dim; j++ ) {
						front->top->colors[i*dim + j] = tempTop->colors[dim - i - 1 + j * dim];
					}
				}
			}
			else if(row > (dim -1)){
				Side *tempTop = new Side(dim, vec4(0, 0, 0, 1));
					for(int i = 0; i< dim*dim; i++){
						tempTop->colors[i] = front->top->colors[i];
					}
					
				for( int i = 0; i < dim; i++ ) {
					for( int j = 0; j < dim; j++ ) {
						front->top->colors[i*dim + j] = tempTop->colors[i + dim * dim - dim * (j + 1 )];
					}
				}	
			}
		}
	}
}