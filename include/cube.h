
#include <Angel.h>

class Cube
{
  
  vec3 * vertices;
  vec3 * normals; 
  int numVertices;

  public:

  Cube()
  {
    //(6 faces)(2 triangles/face)(3 vertices/triangle)
    numVertices = 36;
    vertices = new vec3[numVertices];
    normals = new vec3[numVertices];
    initCube(vertices, normals);
  }

  ~Cube()
  {
    delete[] vertices;
    delete[] normals;
  }

  vec3 * getVertices()
  {
    return vertices;
  }

  vec3 * getNormals()
  {
    return normals;
  }

  int getNumVertices()
  {
    return numVertices;
  }

private:

void initOneFace(int a, int b, int c, int d, vec3 points[], vec3 vertices[], vec3 normals[], int face, int index)
{   
  vec3 normal = normalize(cross(points[c] - points[b], points[a] - points[b]));
 
  vertices[index] = points[a]; 
  normals[index] = normal;
  index++;
 
  vertices[index] = points[b];
  normals[index] = normal;
  index++;
 
  vertices[index] = points[c];
  normals[index] = normal;
  index++;
 
  vertices[index] = points[a];
  normals[index] = normal;
  index++;
 
  vertices[index] = points[c];
  normals[index] = normal;
  index++;
 
  vertices[index] = points[d];
  normals[index] = normal;
}

void initCube(vec3 vertices[], vec3 normals[])
{
  vec3  points[8] = {
  vec3( -0.5, -0.5,  0.5),
  vec3( -0.5,  0.5,  0.5),
  vec3(  0.5,  0.5,  0.5),
  vec3(  0.5, -0.5,  0.5),
  vec3( -0.5, -0.5, -0.5),
  vec3( -0.5,  0.5, -0.5),
  vec3(  0.5,  0.5, -0.5),
  vec3(  0.5, -0.5, -0.5)};

  initOneFace( 1, 0, 3, 2, points, vertices, normals, 0, 0 * 6 ); // z  
  initOneFace( 2, 3, 7, 6, points, vertices, normals, 1, 1 * 6);  // x 
  initOneFace( 3, 0, 4, 7, points, vertices, normals, 2, 2 * 6);  // -y
  initOneFace( 6, 5, 1, 2, points, vertices, normals, 3, 3 * 6);  // y
  initOneFace( 4, 5, 6, 7, points, vertices, normals, 4, 4 * 6);  // -z
  initOneFace( 5, 4, 0, 1, points, vertices, normals, 5, 5 * 6);  // -x
}


};


