//
// Unit square centered at origin in plane z = 0.
// Includes texture coordinates and tangent vectors.
//
#include "Angel.h"

class Square
{
private:

  vec3 * vertices;
  vec3 * normals; 
  vec2 * texCoords;
  vec3 * tangents;
  int numVertices;

public:

  Square(int size = 1)
  {
    numVertices = 6 * size;
    vertices = new vec3[numVertices];
    normals = new vec3[numVertices];
    texCoords = new vec2[numVertices];
    tangents = new vec3[numVertices];
    init(size);
  }

  ~Square()
  {
    delete[] vertices;
    delete[] normals;
    delete[] texCoords;
    delete[] tangents;
  }

  vec3 * getVertices()
  {
    return vertices;
  }

  vec3 * getNormals()
  {
    return normals;
  }

  vec2 * getTexCoords()
  {
    return texCoords;
  }

  vec3 * getTangents()
  {
    return tangents;
  }

  int getNumVertices()
  {
    return numVertices;
  }


private:

void initOneFace(int a, int b, int c, int d, vec3 points[], vec3 vertices[], vec3 normals[], int face, int index)
{   
  vec3 normal = normalize(cross(points[c] - points[b], points[a] - points[b]));
  vec3 tangent = a - b; // t direction

  vertices[index] = points[a]; 
  normals[index] = normal;
  texCoords[index] = vec2(1.0, 1.0);
  tangents[index] = tangent;
  index++;
 
  vertices[index] = points[b];
  normals[index] = normal;
  texCoords[index] = vec2(0.0, 1.0);
  tangents[index] = tangent;
  index++;
 
  vertices[index] = points[c];
  normals[index] = normal;
  texCoords[index] = vec2(0.0, 0.0);
  tangents[index] = tangent;
  index++;
 
  vertices[index] = points[a];
  normals[index] = normal;
  texCoords[index] = vec2(1.0, 1.0);
  tangents[index] = tangent;
  index++;
 
  vertices[index] = points[c];
  normals[index] = normal;
  texCoords[index] = vec2(0.0, 0.0);
  tangents[index] = tangent;
  index++;
 
  vertices[index] = points[d];
  normals[index] = normal;
  texCoords[index] = vec2(1.0, 0.0);
  tangents[index] = tangent;

}

void init(int size)
{
  vec3  points[4] = {
  vec3( -0.5, -0.5,  0.0),
  vec3( -0.5,  0.5,  0.0),
  vec3(  0.5,  0.5,  0.0),
  vec3(  0.5, -0.5,  0.0)};

  initOneFace( 1, 0, 3, 2, points, vertices, normals, 0, 0 * 6 );  

  // for additional squares, shift points one unit to left (-y direction)
  for (int i = 1; i < size; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      points[j][1] -= 1.0;
    }
    initOneFace( 1, 0, 3, 2, points, vertices, normals, 0, i * 6 );  
  }
}


};


