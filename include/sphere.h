#include <Angel.h>

class Sphere
{
  vec3 * vertices; 
  vec3 * normals; 
  vec3 * tangents;
  vec2 * texCoords;

  int numVertices;
  int Index;
  bool trueNormals;
  bool useStereographic;
  int repeat;

public:

  Sphere(int nn = 4, bool useTrueNormals = false)
  {
    // 4n x 2n quads, each quad is 6 vertices
    trueNormals = useTrueNormals;
    useStereographic = false;
    //repeat = 1;
    repeat = 4;
    numVertices = 48 * nn * nn;
    vertices = new vec3[numVertices];
    normals = new vec3[numVertices];
    tangents = new vec3[numVertices];
    texCoords = new vec2[numVertices];
    Index = 0;
    generate(nn);
  }

  vec3 * getVertices()
  {
    return vertices;
  }

  vec3 * getNormals()
  {
    return normals;
  }

  vec3 * getTangents()
  {
    return tangents;
  }

  vec2* getTexCoords()
  {
    return texCoords;
  }

  int getNumVertices()
  {
    return numVertices;
  }

  ~Sphere()
  {
    delete[] vertices;
    delete[] normals;
    delete[] tangents;
    delete[] texCoords;
  }

  // convert to rectangular
  vec3 convert(GLfloat theta, GLfloat phi)
  {
    return vec3(sin(phi) * cos(theta),
                sin(phi) * sin(theta),
                cos(phi));
  }

  // for point p at azimuth angle theta, return the point on the stereographic projection
  vec2 stereographic(GLfloat theta, vec3 p)
  {
    //  (0, 0, -1) + alpha * (p[0], p[1], p[2] + 1) = (x, y, 1)
    GLfloat alpha = 2.0 / (p[2] + 1);
    GLfloat x = alpha * p[0];
    GLfloat y = alpha * p[1];

    // result is in the range [-2, 2], so scale by .25 and add .5
    GLfloat s = x * 0.25 + 0.5;
    GLfloat t = y * 0.25 + 0.5;
    return vec2(s, t);
  }

  // for point p at azimuth angle theta, return the point on the cylindrical projection
  vec2 cylindrical(GLfloat theta, vec3 p)
  {
    //GLfloat s = theta / (2 * M_PI);
    GLfloat s = theta / (2 * M_PI) * repeat;  // map texture to 1/repeat of circumference
    //GLfloat t = p[2];
    GLfloat t = p[2] / 2.0 + 0.5; // upper hemisphere to upper half of texture
    return vec2(s, t);
  }

  void generate(int n)
  {
    // 4n points around equator, n points up meridians
    int index = 0;
    GLfloat delta = 2.0 * M_PI / (4 * n);
    //GLfloat deltaPhi = 2.0 * M_PI / n;
    GLfloat theta, phiPrime;
    for (int i = 0; i < 4 * n; ++i)
    {
      theta = i * delta;
      for (int j = 0; j < n - 1; ++j)
      {
        phiPrime = j * delta;
        quad(theta, theta + delta, phiPrime, phiPrime + delta);
      }
      // north pole
      phiPrime = (n - 1) * delta;
      quad(theta, theta + delta, phiPrime, M_PI / 2.0);
    }
    // international date line
    //theta = ((4 * n) - 1) * delta;
    //for (int j = 0; j < n - 1; ++j)
    //{
    //  phiPrime = j * delta;
    //  quad(theta, 0.0, phiPrime, phiPrime + delta);
    //}
    //phiPrime = (n - 1) * delta;
    //quad(theta, 0.0, phiPrime, M_PI / 2.0);

    // southern hemisphere
    const int half = Index;
    for (int i = 0; i < half; ++i)
    {
      vec3 northern = vertices[i];
      vertices[i + half] = vec3(northern[0], northern[1], -northern[2]);
      //normals[i + half] = vertices[i + half];
      normals[i + half] = vec3(normals[i][0], normals[i][1], -normals[i][2]); 
      tangents[i + half] = tangents[i];  // directly south, so tangent is the same direction
      vec2 tex = texCoords[i];
      //texCoords[i + half] = vec2(tex[0], tex[1] - 0.5);
      if (useStereographic)
      {
        texCoords[i + half] = texCoords[i];
      }
      else
      {
        texCoords[i + half] = vec2(tex[0], 1.0 - tex[1]);
      }
    }
  }

  void quad(GLfloat theta, GLfloat theta2, GLfloat phi, GLfloat phi2)
  {
    //vec2 (*proj)(GLfloat theta, vec3 p);
    //proj = cylindrical;

    vec3 a = convert(theta, M_PI / 2.0 - phi);
    vec3 b = convert(theta2, M_PI / 2.0 - phi);
    vec3 c = convert(theta2, M_PI / 2.0 - phi2);
    vec3 d = convert(theta, M_PI / 2.0 - phi2);

    vec3 fn = cross(b - a, d - a);

    //std::cout << a << std::endl;

    // get a tangent vector by taking a cross product with something pointing up 
    // such as (0, 0, 2)
    vec3 up(0.0, 0.0, 2.0);
    vec3 aTangent = normalize(cross(up, a));
    vec3 bTangent = normalize(cross(up, b));
    vec3 cTangent = normalize(cross(up, c));
    vec3 dTangent = normalize(cross(up, d));

    // two triangles
    vertices[Index] = a;
    texCoords[Index] = useStereographic ? stereographic(theta, a) : cylindrical(theta, a);
    normals[Index] = trueNormals ? a : fn;
    tangents[Index] = aTangent;

    ++Index;
    vertices[Index] = b;
    texCoords[Index] = useStereographic ? stereographic(theta2, b) : cylindrical(theta2, b);
    normals[Index] = trueNormals ? b : fn;
    tangents[Index] = bTangent;
    ++Index;
    vertices[Index] = c;
    texCoords[Index] = useStereographic ? stereographic(theta2, c) : cylindrical(theta2, c);
    normals[Index] = trueNormals ? c : fn;
    tangents[Index] = cTangent;
    ++Index;
    vertices[Index] = a;
    texCoords[Index] = useStereographic ? stereographic(theta, a) : cylindrical(theta, a);
    normals[Index] = trueNormals ? a : fn;
    tangents[Index] = aTangent;
    ++Index;
    vertices[Index] = c;
    texCoords[Index] = useStereographic ? stereographic(theta2, c) : cylindrical(theta2, c);
    normals[Index] = trueNormals ? c : fn;
    tangents[Index] = cTangent;
    ++Index;
    vertices[Index] = d;
    texCoords[Index] = useStereographic ? stereographic(theta, d) : cylindrical(theta, d);
    normals[Index] = trueNormals ? d: fn;
    tangents[Index] = dTangent;
    ++Index;
  }



void generateTexCoords()
{
  // cylindrical projection
  for (int i = 0; i < getNumVertices(); ++i)
  {
    vec3 v = vertices[i];
    GLfloat x = v[0];
    GLfloat y = v[1];
    GLfloat z = v[2];
    GLfloat r = sqrt(1 - z * z);
    if (r < DivideByZeroTolerance)
    {
      texCoords[i] = vec2(0, 0);
    }
    else
    {
      GLfloat angle;
      GLfloat refAngle = acos(abs(x) / r);
      if (x >= 0 && y >= 0)
      {
        angle = refAngle;
      }
      else if (x < 0 && y >= 0)
      {
        angle = M_PI - refAngle;
      }
      else if (x < 0 && y < 0)
      {
        angle = M_PI + refAngle;
      }
      else
      {
        angle = 2 * M_PI - refAngle;
      }
      GLfloat t = (z + 1.0) / 2.0;
      GLfloat s = angle / (2 * M_PI);
      texCoords[i] = vec2(s, t);
    }

  }
}

};