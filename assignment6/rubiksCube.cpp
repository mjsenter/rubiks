#include "rubiksCube.h"


rubiksCube::rubiksCube( int dimensions ) {
	srand(time(NULL));

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
	anim->numFrames = 15;
	anim->transform = Scale( 1.0 );

	dim = dimensions;
	cursor = 0;

	isScrambled = false;

	front = createSides( dimensions );
	nextFront = createSides( dimensions );

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

rubiksCube::Side * rubiksCube::createSides( int dimensions ) {
	Side * of_the_king;
	//Front face setup
	of_the_king = new Side( dimensions, colors[0] );
	of_the_king->back = new Side( dimensions, colors[1] );
	of_the_king->top = new Side( dimensions, colors[2] );
	of_the_king->bottom = new Side( dimensions, colors[3] );
	of_the_king->right = new Side( dimensions, colors[4] );
	of_the_king->left = new Side( dimensions, colors[5] );
	
	//Back face setup
	of_the_king->back->back = of_the_king;
	of_the_king->back->top = of_the_king->bottom;
	of_the_king->back->right = of_the_king->right;
	of_the_king->back->left = of_the_king->left;
	of_the_king->back->bottom = of_the_king->top;

	//Top face setup
	of_the_king->top->back = of_the_king->bottom;
	of_the_king->top->top = of_the_king->back;
	of_the_king->top->right = of_the_king->right;
	of_the_king->top->left = of_the_king->left;
	of_the_king->top->bottom = of_the_king;

	//Bottom face setup
	of_the_king->bottom->back = of_the_king->top;
	of_the_king->bottom->top = of_the_king;
	of_the_king->bottom->right = of_the_king->right;
	of_the_king->bottom->left = of_the_king->left;
	of_the_king->bottom->bottom = of_the_king->back;

	//Right face setup
	of_the_king->right->back = of_the_king->left;
	of_the_king->right->top = of_the_king->top;
	of_the_king->right->right = of_the_king->back;
	of_the_king->right->left = of_the_king;
	of_the_king->right->bottom = of_the_king->bottom;

	//Left face setup
	of_the_king->left->back = of_the_king->right;
	of_the_king->left->top = of_the_king->bottom;
	of_the_king->left->right = of_the_king->back;
	of_the_king->left->left = of_the_king;
	of_the_king->left->bottom = of_the_king->top;

	return of_the_king;
}

rubiksCube::Side::Side( int dim, vec4 color ) {
	numColors = dim * dim;
	colors = (vec4 *)malloc( sizeof( vec4 ) * numColors );
	rotate = (bool *)malloc( sizeof( bool ) * numColors );
	for( int i = 0; i < numColors; i++ ) {
		colors[i] = color;
		rotate[i] = false;
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
	//Draw faces
	drawFace( view, proj, 0, front, true );
	drawFace( view, proj, 1, front->back, false );
	drawFace( view, proj, 2, front->top, false );
	drawFace( view, proj, 3, front->bottom, false );
	drawFace( view, proj, 4, front->right, false );
	drawFace( view, proj, 5, front->left, false );
}

void rubiksCube::drawFace( mat4 view, mat4 proj, int rot, Side * side, bool drawCursor ) {
	mat4 colorScale = Scale( 1 / (GLfloat)dim * 0.9 );
	mat4 cubeScale = Scale( 1 / (GLfloat)dim );
	for( int i = 0; i < dim; i++ ) {
		for( int j = 0; j < dim; j++ ) {
			mat4 rotModel;
			switch( rot ) {
				case 1:
					rotModel = RotateY( 180 );
					break;
				case 2:
					rotModel = RotateX( -90 );
					break;
				case 3:
					rotModel = RotateX( 90 );
					break;
				case 4:
					rotModel = RotateY( 90 );
					break;
				case 5:
					rotModel = RotateY( -90 );
					break;
			}
			if( anim->rotate ) {
				if( side->rotate[i*dim + j] ) {
					rotModel = anim->transform * rotModel;
				}
			}

			mat4 model = Translate( (GLfloat)1/(2*dim) + 
					(GLfloat)j/dim, -(GLfloat)1/(2*dim) - 
					(GLfloat)i/dim, 0.0 ) * 
				Translate( -0.5, 0.5, 0.51 ) * 
				colorScale;
			faceShader->Bind();
			face->Bind( *faceShader );
			faceShader->SetUniform( "color", side->colors[i*dim + j] );
			faceShader->SetUniform( "model", rotModel * model );
			faceShader->SetUniform( "cursor", (cursor == i*dim + j) && drawCursor);
			faceShader->SetUniform( "view", view );
			faceShader->SetUniform( "projection", proj );
			face->Draw( GL_TRIANGLE_FAN );
			face->Unbind();
			faceShader->Unbind();

			model = Translate( (GLfloat)1/(2*dim) + 
						(GLfloat)j/dim, -(GLfloat)1/(2*dim) - 
						(GLfloat)i/dim, -(GLfloat)1/(2*dim) ) * 
					Translate( -0.5, 0.5, 0.5 ) * 
					cubeScale;
			baseShader->Bind();
			baseShader->SetUniform( "view", view );
			baseShader->SetUniform( "model", rotModel * model );
			baseShader->SetUniform( "projection", proj );
			baseShader->SetUniform( "color", vec4( 0.0, 0.0, 0.0, 1.0 ) );
			baseCube->Bind( *baseShader );
			baseCube->Draw( GL_TRIANGLES );
			baseCube->Unbind();
			baseShader->Unbind();
		}
	}
}

void rubiksCube::rotate(bool v, bool d) {
	if( anim->rotate ) { //already rotating
		return;
	}
	anim->rotate = true;
	anim->vert = v;
	anim->dir = d;
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
		if(v && d){
			for(int i = 0; i < dim; i++){
				nextFront->colors[column + i*dim] = front->bottom->colors[column + i*dim];
				nextFront->bottom->colors[column + i*dim] = front->back->colors[(dim*dim - 1) - (column + i*dim)];
				nextFront->back->colors[(dim*dim - 1) - (column + i*dim)] = front->top->colors[column + i*dim];
				nextFront->top->colors[column + i*dim] = tempFront->colors[column + i*dim];

				front->rotate[column + i*dim] = true;
				front->top->rotate[column + i*dim] = true;
				front->back->rotate[(dim*dim - 1) - (column + i*dim)] = true;
				front->bottom->rotate[column +i*dim] = true;
			}


			if(column == 0){
				Side *tempLeft = new Side(dim, vec4(0, 0, 0, 1));
					for(int i = 0; i< dim*dim; i++){
						tempLeft->colors[i] = front->left->colors[i];
					}

				for( int i = 0; i < dim; i++ ) {
					for( int j = 0; j < dim; j++ ) {
						nextFront->left->colors[i*dim + j] = tempLeft->colors[dim - i - 1 + j * dim];
						front->left->rotate[i*dim + j] = true;
					}
				}
			}

			else if(column == (dim -1)){
				Side *tempRight = new Side(dim, vec4(0, 0, 0, 1));
					for(int i = 0; i< dim*dim; i++){
						tempRight->colors[i] = front->right->colors[i];
					}
					
				for( int i = 0; i < dim; i++ ) {
					for( int j = 0; j < dim; j++ ) {
						nextFront->right->colors[i*dim + j] = tempRight->colors[i + dim * dim - dim * (j + 1 )];		
						front->right->rotate[i*dim + j] = true;
					}
				}
			}
			}
		}

		//rotate down
		if(v && !d){
			for(int i = 0; i < dim; i++){
				nextFront->colors[column + i*dim] = front->top->colors[column + i*dim];
				nextFront->top->colors[column + i*dim] = front->back->colors[(dim*dim - 1) - (column + i*dim)];
				nextFront->back->colors[(dim*dim - 1) - (column + i*dim)] = front->bottom->colors[column + i*dim];
				nextFront->bottom->colors[column + i*dim] = tempFront->colors[column + i*dim];

				front->rotate[column + i*dim] = true;
				front->top->rotate[column + i*dim] = true;
				front->back->rotate[(dim*dim - 1) - (column + i*dim)] = true;
				front->bottom->rotate[column + i*dim] = true;
				
			}

			if(column == 0){
			Side *tempRight = new Side(dim, vec4(0, 0, 0, 1));
				for(int i = 0; i< dim*dim; i++){
					tempRight->colors[i] = front->left->colors[i];
				}
					
				for( int i = 0; i < dim; i++ ) {
					for( int j = 0; j < dim; j++ ) {
						nextFront->left->colors[i*dim + j] = tempRight->colors[i + dim * dim - dim * (j + 1 )];		
						front->left->rotate[i*dim + j] = true;
					}
				}
			}

			else if(column == (dim - 1)){
				Side *tempRight = new Side(dim, vec4(0, 0, 0, 1));
					for(int i = 0; i< dim*dim; i++){
						tempRight->colors[i] = front->right->colors[i];
					}

				for( int i = 0; i < dim; i++ ) {
					for( int j = 0; j < dim; j++ ) {
						nextFront->right->colors[i*dim + j] = tempRight->colors[dim - i - 1 + j * dim];
						front->right->rotate[i*dim + j] = true;
					}
				}
			}
		}
	
	//Horizontal
	if(!v) {
		//rotate right
		if(!v && d) {
			for(int i = 0; i < dim; i++) {
				nextFront->colors[row*dim + i] = front->left->colors[row*dim + i];
				nextFront->left->colors[row*dim + i] = front->back->colors[row*dim + i];
				nextFront->back->colors[row*dim + i] = front->right->colors[row*dim + i];
				nextFront->right->colors[row*dim + i] = tempFront->colors[row*dim + i];

				front->rotate[row*dim + i] = true;
				front->left->rotate[row*dim + i] = true;
				front->back->rotate[row*dim + i] = true;
				front->right->rotate[row*dim + i] = true;
				
			}
			
			if(row == 0){
				Side *tempTop = new Side(dim, vec4(0, 0, 0, 1));
					for(int i = 0; i< dim*dim; i++){
						tempTop->colors[i] = front->top->colors[i];
					}

				for( int i = 0; i < dim; i++ ) {
					for( int j = 0; j < dim; j++ ) {
						nextFront->top->colors[i*dim + j] = tempTop->colors[dim - i - 1 + j * dim];
						
						front->top->rotate[i*dim + j] = true;
					}
				}
			}

			else if(row == (dim - 1)){
				Side *tempTop = new Side(dim, vec4(0, 0, 0, 1));
					for(int i = 0; i< dim*dim; i++){
						tempTop->colors[i] = front->bottom->colors[i];
					}

				for( int i = 0; i < dim; i++ ) {
					for( int j = 0; j < dim; j++ ) {
						nextFront->bottom->colors[i*dim + j] = tempTop->colors[i + dim * dim - dim * (j + 1 )];				
						front->bottom->rotate[i*dim + j] = true;
					}
				}
			}
		}

		//rotate left
		if(!v && !d){

			for(int i = 0; i < dim; i++){
				nextFront->colors[row*dim + i] = front->right->colors[row*dim + i];
				nextFront->right->colors[row*dim + i] = front->back->colors[row*dim + i];

				/*nextFront->right->colors[row*dim + i] = front->back->colors[(dim - row)*dim - i - 1];*/
				//nextFront->back->colors[(dim - row)*dim - i - 1] = front->left->colors[(dim - row)*dim - i - 1];
				//nextFront->left->colors[(dim - row)*dim - i - 1] = tempFront->colors[row*dim + i];

				nextFront->back->colors[row*dim + i] = front->left->colors[row*dim + i];
				nextFront->left->colors[row*dim + i] = tempFront->colors[row*dim + i];


				front->rotate[row*dim + i] = true;
				front->left->rotate[row*dim + i] = true;
				front->back->rotate[row*dim + i] = true;
				front->right->rotate[row*dim + i] = true;
			}

			if(row == 0){
				Side *tempTop = new Side(dim, vec4(0, 0, 0, 1));
					for(int i = 0; i< dim*dim; i++){
						tempTop->colors[i] = front->top->colors[i];
					}

				for( int i = 0; i < dim; i++ ) {
					for( int j = 0; j < dim; j++ ) {
						nextFront->top->colors[i*dim + j] = tempTop->colors[i + dim * dim - dim * (j + 1 )];
						front->top->rotate[i*dim + j] = true;
					}
				}
			}
			else if(row == (dim - 1)){
				Side *tempTop = new Side(dim, vec4(0, 0, 0, 1));
					for(int i = 0; i< dim*dim; i++){
						tempTop->colors[i] = front->bottom->colors[i];
					}
					
				for( int i = 0; i < dim; i++ ) {
					for( int j = 0; j < dim; j++ ) {
						nextFront->bottom->colors[i*dim + j] = tempTop->colors[dim - i - 1 + j * dim];
						front->bottom->rotate[i*dim + j] = true;
					}
				}
			}
		}
	}
}

void rubiksCube::rotateCube( bool v, bool d ) {
	if(anim->rotate){
		return;
	}
	Side * tempFront = front;
	int tempCursor = cursor;
	if( v ) {
		if( d ) {
			for(int i = 0; i < dim; i++){
				cursor = i;
				rotate(1, 0);
				anim->rotate = false;
			}
		}
		else {
			for(int i = 0; i < dim; i++){
				cursor = i;
				rotate(1, 1);
				anim->rotate = false;
			}
		}
	}
	else {
		if( d ) {
			for(int i = 0; i < dim; i++){
				cursor = i*dim;
				rotate( 0, 0);
				anim->rotate = false;
			}
		}
		else {
			for(int i = 0; i < dim; i++) {
				cursor = i*dim;
				rotate(0, 1);
				anim->rotate = false;
			}	
		}
	}
	anim->rotate = true;
	cursor = tempCursor;
}

void rubiksCube::scramble() {
	if(anim->rotate){
		return;
	}
	int tempCursor, vert, direction;
	tempCursor = cursor;
		cursor = random % dim*dim + 1;
		vert = random % 2;
		direction = random % 2;
		rotate(vert, direction);
	cursor = tempCursor;
	isScrambled = true;
}

bool rubiksCube::isWin() {
	if( !isScrambled ) {
		return false;
	}

	//checks front face
	for(int i = 1; i<dim*dim; i++){
		if(nextFront->colors[i].x != nextFront->colors[i - 1].x
			|| nextFront->colors[i].y != nextFront->colors[i - 1].y
			|| nextFront->colors[i].z != nextFront->colors[i - 1].z
			|| nextFront->colors[i].w != nextFront->colors[i - 1].w){
				return false;
		}
	}
	//checks right face
	for(int i = 1; i<dim*dim; i++){
		if(nextFront->right->colors[i].x != nextFront->right->colors[i - 1].x
			|| nextFront->right->colors[i].y != nextFront->right->colors[i - 1].y
			|| nextFront->right->colors[i].z != nextFront->right->colors[i - 1].z
			|| nextFront->right->colors[i].w != nextFront->right->colors[i - 1].w){
				return false;
		}
	}
	//checks top face
	for(int i = 1; i<dim*dim; i++){
		if(nextFront->top->colors[i].x != nextFront->top->colors[i - 1].x
			|| nextFront->top->colors[i].y != nextFront->top->colors[i - 1].y
			|| nextFront->top->colors[i].z != nextFront->top->colors[i - 1].z
			|| nextFront->top->colors[i].w != nextFront->top->colors[i - 1].w){
				return false;
		}
	}
	//checks left face
	for(int i = 1; i<dim*dim; i++){
		if(nextFront->left->colors[i].x != nextFront->left->colors[i - 1].x
			|| nextFront->left->colors[i].y != nextFront->left->colors[i - 1].y
			|| nextFront->left->colors[i].z != nextFront->left->colors[i - 1].z
			|| nextFront->left->colors[i].w != nextFront->left->colors[i - 1].w){
				return false;
		}
	}
	//checks back face
	for(int i = 1; i<dim*dim; i++){
		if(nextFront->back->colors[i].x != nextFront->back->colors[i - 1].x
			|| nextFront->back->colors[i].y != nextFront->back->colors[i - 1].y
			|| nextFront->back->colors[i].z != nextFront->back->colors[i - 1].z
			|| nextFront->back->colors[i].w != nextFront->back->colors[i - 1].w){
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
	random = glutGet(GLUT_ELAPSED_TIME);
	if( anim->vert ) { //Vertical rotation
		if( anim->dir ) { //Up
			anim->transform = RotateX( (float)-90 / anim->numFrames ) * anim->transform;
		}
		else { //Down
			anim->transform = RotateX( (float)90 / anim->numFrames ) * anim->transform;
		}
	}
	else { //Horizontal rotation
		if( anim->dir ) { //Right
			anim->transform = RotateY( (float)90 / anim->numFrames ) * anim->transform;
		}
		else { //Left
			anim->transform = RotateY( (float)-90 / anim->numFrames ) * anim->transform;
		}
	}
	
	anim->count++;
	if( anim->count >= anim->numFrames ) {
		Side * oldFront = front;
		front = nextFront;
		
		delete oldFront->back;
		delete oldFront->bottom;
		delete oldFront->top;
		delete oldFront->left;
		delete oldFront->right;
		delete oldFront;

		nextFront = createSides( dim );
		for( int i = 0; i < dim * dim; i++ ) {
			nextFront->colors[i] = front->colors[i];
			nextFront->back->colors[i] = front->back->colors[i];
			nextFront->top->colors[i] = front->top->colors[i];
			nextFront->bottom->colors[i] = front->bottom->colors[i];
			nextFront->right->colors[i] = front->right->colors[i];
			nextFront->left->colors[i] = front->left->colors[i];
		}

		anim->count = 0;
		anim->rotate = false;
		anim->transform = Scale( 1.0 );
	}
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

			if(column == (dim - 1)){
					Side *tempRight = new Side(dim, vec4(0, 0, 0, 1));
					for(int i = 0; i< dim*dim; i++){
						tempRight->colors[i] = front->right->colors[i];
					}

				for( int i = 0; i < dim; i++ ) {
					for( int j = 0; j < dim; j++ ) {
						front->right->colors[i*dim + j] = tempRight->colors[dim - i - 1 + j * dim];
						front->right->rotate[i*dim + j];
					}
				}				
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
						front->top->rotate[i*dim + j] = true;
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
						front->bottom->colors[i*dim + j] = tempTop->colors[dim - i - 1 + j * dim];
						front->bottom->rotate[i*dim + j] = true;
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
						front->top->rotate[i*dim + j] = true;
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
						front->bottom->rotate[i*dim + j] = true;
					}
				}	
			}
		}
	}
}