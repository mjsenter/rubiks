#include <cassert>
#include <math.h>
#include <string.h>
#include "Matrix.h"

// Define pi if it isn't already
#ifndef M_PI
  #define M_PI 3.1415926535897932384626433832795028841971693993751058209
#endif

// Macro for easy degree to radian conversion
#define DEG2RAD(x) (x * (float)(M_PI / 180.0))

/*
 * The identity matrix
 */
const Matrix Matrix::Identity(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f);

/*
 * Default constructor
 */
Matrix::Matrix()
{
    memset(this->data, (int)0.0f, sizeof(this->data));
}

/*
 * Copy constructor
 */
Matrix::Matrix(const Matrix& other)
{
    memcpy(this->data, other.data, sizeof(this->data));
}

/*
 * 16 float constructor
 */
Matrix::Matrix(float row0col0, float row0col1, float row0col2, float row0col3,
               float row1col0, float row1col1, float row1col2, float row1col3,
               float row2col0, float row2col1, float row2col2, float row2col3,
               float row3col0, float row3col1, float row3col2, float row3col3)
{
    data[0]  = row0col0;
    data[1]  = row0col1;
    data[2]  = row0col2;
    data[3]  = row0col3;

    data[4]  = row1col0;
    data[5]  = row1col1;
    data[6]  = row1col2;
    data[7]  = row1col3;

    data[8]  = row2col0;
    data[9]  = row2col1;
    data[10] = row2col2;
    data[11] = row2col3;

    data[12] = row3col0;
    data[13] = row3col1;
    data[14] = row3col2;
    data[15] = row3col3;
}

/*
 * Float array constructor
 */
Matrix::Matrix(const float* data)
{
    memcpy(this->data, data, sizeof(this->data));
}

/*
 * Destructor
 */
Matrix::~Matrix()
{
    //Do nothing
}

/*
 * Inverse
 */
Matrix Matrix::Inverse() const
{
    // TODO
    return Matrix::Identity;
}

/*
 * Transpose
 */
Matrix Matrix::Transpose() const
{
    Matrix ret;

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            ret(row, col) = (*this)(col, row);
        }
    }

    return ret;
}

float Matrix::Determinant() const
{
    //TODO
    return 0;
}

/*
 * Transform
 */
Vector3D Matrix::Transform(const Vector3D& vec3d) const
{
    // Technically, we can only multiply a 4x4 matrix with a 4D vector
    // so we promote the 3D vector by assuming a w value of 1
    return ((*this) * Vector4D(vec3d, 1.0f)).XYZ();
}

/*
 * Transform Normal
 */
Vector3D Matrix::TransformNormal(const Vector3D& vec3d) const
{
    // Same as above, but use 0 for the homogeneous coordinate to prevent translation
    return ((*this) * Vector4D(vec3d, 0.0f)).XYZ();
}

/*
 * Multiply vec4d
 */
Vector4D Matrix::operator*(const Vector4D& vec4d) const
{
    Vector4D ret;
    for (int row = 0; row < 4; row++)
    {
        Vector4D currentRow(
            (*this)(row, 0), 
            (*this)(row, 1), 
            (*this)(row, 2), 
            (*this)(row, 3));
        ret[row] = vec4d.Dot(currentRow);
    }
    return ret;
}

/*
 * Multiply scalar
 */
Matrix Matrix::operator*(float scalar) const
{
    // Copy and then apply the multiplication to the copy
    Matrix ret(*this);

    for (int i = 0; i < 16; i++)
    {
        ret.data[i] *= scalar;
    }

    return ret;
}

/*
 * Addition operator
 */
Matrix Matrix::operator+(const Matrix& other) const
{
    Matrix ret;
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            ret(row, col) = (*this)(row, col) + other(row, col);
        }
    }
    return ret;
}

/*
 * Subtraction operator
 */
Matrix Matrix::operator-(const Matrix& other) const
{
    Matrix ret;
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            ret(row, col) = (*this)(row, col) - other(row, col);
        }
    }
    return ret;
}

/*
 * Multiplication operator
 */
Matrix Matrix::operator*(const Matrix& other) const
{
    Matrix ret; //Zero initialized

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            for (int k = 0; k < 4; k++)
            {
                ret(row, col) += (*this)(row, k) * other(k, col);
            }
        }
    }

    return ret;
}

/*
 * Assignment operator
 */
Matrix& Matrix::operator=(const Matrix& other)
{
    memcpy(this->data, other.data, sizeof(this->data));

    return *this;
}

/*
 * Multiply assignment by scalar
 */
Matrix& Matrix::operator*=(float scalar)
{
    return (*this) = (*this) * scalar;
}

/*
 * Addition assignment operator
 */
Matrix& Matrix::operator+=(const Matrix& other)
{
    return (*this) = (*this) + other;
}

/*
 * Subtraction assignment operator
 */
Matrix& Matrix::operator-=(const Matrix& other)
{
    return (*this) = (*this) - other;
}

/*
 * Parenthesis index operator
 */
float& Matrix::operator()(int row, int col)
{
    assert(row >= 0 && row < 4);
    assert(col >= 0 && col < 4);

    // Row-major ordering
    return data[row*4 + col];
}

/*
 * Constant parenthesis index operator
 */
const float& Matrix::operator()(int row, int col) const
{
    assert(row >= 0 && row < 4);
    assert(col >= 0 && col < 4);

    // Row-major ordering
    return data[row*4 + col];
}

/*
 * Get translation matrix from Vector3D
 */
Matrix Matrix::GetTranslate(const Vector3D& vec3d)
{
    return Matrix::GetTranslate(vec3d[0], vec3d[1], vec3d[2]);
}

/*
 * Get translation matrix from floats
 */
Matrix Matrix::GetTranslate(float x, float y, float z)
{
    Matrix ret(Matrix::Identity);

    ret(0, 3) = x;
    ret(1, 3) = y;
    ret(2, 3) = z;

    return ret;
}

/*
 * Rotate X
 */
Matrix Matrix::GetRotateX(float degrees)
{
    Matrix ret; // Zero initialized

    float radians = DEG2RAD(degrees);
    float cosine  = cos(radians);
    float sine    = sin(radians);

    ret(0,0) = 1.0f;
    ret(1,1) = cosine;
    ret(1,2) = -sine;
    ret(2,1) = sine;
    ret(2,2) = cosine;
    ret(3,3) = 1.0f;

    return ret;
}

/*
 * Rotate Y
 */
Matrix Matrix::GetRotateY(float degrees)
{
    Matrix ret; // Zero initialized

    float radians = DEG2RAD(degrees);
    float cosine  = cos(radians);
    float sine    = sin(radians);

    ret(0,0) = cosine;
    ret(0,2) = sine;
    ret(1,1) = 1.0f;
    ret(2,0) = -sine;
    ret(2,2) = cosine;
    ret(3,3) = 1.0f;

    return ret;
}

/*
 * Rotate Z
 */
Matrix Matrix::GetRotateZ(float degrees)
{
    Matrix ret; // Zero initialized

    float radians = DEG2RAD(degrees);
    float cosine  = cos(radians);
    float sine    = sin(radians);

    ret(0,0) = cosine;
    ret(0,1) = -sine;
    ret(1,0) = sine;
    ret(1,1) = cosine;
    ret(2,2) = 1.0f;
    ret(3,3) = 1.0f;

    return ret;
}

/*
 * Get rotation matrix
 */
Matrix Matrix::GetRotate(const Vector3D& axis, float degrees)
{
    // Algorithm adapted from: http://www.arcsynthesis.org/gltut/Positioning/Tut06%20Rotation.html

    Matrix ret; //zero initialized

    float radians    = DEG2RAD(degrees);
    float cosine     = cos(radians);
    float cosineComp = 1 - cosine;
    float sine       = sin(radians);

    ret(0,0) = axis[0] * axis[0] + (1 - axis[0] * axis[0]) * cosine;
    ret(0,1) = cosineComp * axis[0] * axis[1] - axis[2] * sine;
    ret(0,2) = cosineComp * axis[0] * axis[2] + axis[1] * sine;
    ret(1,0) = cosineComp * axis[0] * axis[1] + axis[2] * sine;
    ret(1,1) = axis[1] * axis[1] + (1 - axis[1] * axis[1]) * cosine;
    ret(1,2) = cosineComp * axis[1] * axis[2] - axis[0] * sine;
    ret(2,0) = cosineComp * axis[0] * axis[2] - axis[1] * sine;
    ret(2,1) = cosineComp * axis[1] * axis[2] + axis[0] * sine;
    ret(2,2) = axis[2] * axis[2] + (1 - axis[2] * axis[2]) * cosine;
    ret(3,3) = 1.0f;

    return ret;
}

/*
 * Get uniform scale matrix
 */
Matrix Matrix::GetScale(float scale)
{
    return Matrix::GetScale(scale, scale, scale);
}

/*
 * Get scale matrix from vector3D
 */
Matrix Matrix::GetScale(const Vector3D& scale)
{
    return Matrix::GetScale(scale[0], scale[1], scale[2]);
}

/*
 * Get scale matrix from floats
 */
Matrix Matrix::GetScale(float x, float y, float z)
{
    Matrix ret;

    ret(0, 0) = x;
    ret(1, 1) = y;
    ret(2, 2) = z;
    ret(3, 3) = 1.0f;

    return ret;
}

/*
 * Get projection matrix
 */
Matrix Matrix::GetProjection(float near, float far, float fovY, float aspectRatio)
{
    // Algorithm adapted from: http://www.songho.ca/opengl/gl_projectionmatrix.html
    
    Matrix ret;
    float fovYrad = DEG2RAD(fovY);
    
    float top   = tan(fovYrad/2) * near;
    float right = top * aspectRatio;

    ret(0,0) = near / right;
    ret(1,1) = near / top;
    ret(2,2) = -1*(far + near) / (far - near);
    ret(2,3) = -2*far*near / (far - near);
    ret(3,2) = -1.0f;

    return ret;
}

/*
 * Get view matrix
 */
Matrix Matrix::GetView(const Vector3D& from, 
                       const Vector3D& at,
                       const Vector3D& up)
{
    // Derive the basis vectors
    Vector3D forward = (at - from).Normalize();
    Vector3D right   = forward.Cross(up).Normalize();
    Vector3D realUp  = right.Cross(forward).Normalize();

    // Build the rotation matrix from the basis vectors
    Matrix ret(right[0],    right[1],    right[2],   0.0f,
               realUp[0],   realUp[1],   realUp[2],  0.0f,
              -forward[0], -forward[1], -forward[2], 0.0f,
               0.0f,        0.0f,        0.0f,       1.0f);

    // Translate so the camera is at the origin, then apply the rotation
    return ret * GetTranslate(-from);
}

/*
 * Get world matrix
 */
Matrix Matrix::GetWorld(const Vector3D& position, 
                        const Vector3D& heading, 
                        const Vector3D& up)
{
    //TODO
    return Matrix::Identity;
}

/*
 * Get orthographic matrix from floats
 */
Matrix Matrix::GetOrtho(float minX, float minY, float minZ,
                        float maxX, float maxY, float maxZ)
{
    return Matrix::GetScale(2 / (maxX - minX), 
                            2 / (maxY - minY), 
                            2 / (maxZ - minZ))  *
           Matrix::GetTranslate(-(maxX - minX) / 2, 
                                -(maxY - minY) / 2, 
                                -(maxZ - minZ) / 2);
}

/*
 * Get orthographic matrix from vectors
 */
Matrix Matrix::GetOrtho(const Vector3D& minXYZ, const Vector3D& maxXYZ)
{
    return Matrix::GetOrtho(minXYZ[0], minXYZ[1], minXYZ[2],
                            maxXYZ[0], maxXYZ[1], maxXYZ[2]);
}
