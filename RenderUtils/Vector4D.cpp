#include <cassert>
#include <math.h>
#include "Vector4D.h"

/*
 * Zero vector
 */
const Vector4D Vector4D::Zero = Vector4D(0.0f, 0.0f, 0.0f, 0.0f);

/*
 * Unit x vector
 */
const Vector4D Vector4D::UnitX = Vector4D(1.0f, 0.0f, 0.0f, 0.0f);

/*
 * Unit y vector
 */
const Vector4D Vector4D::UnitY = Vector4D(0.0f, 1.0f, 0.0f, 0.0f);

/*
 * Unit z vector
 */
const Vector4D Vector4D::UnitZ = Vector4D(0.0f, 0.0f, 1.0f, 0.0f);

/*
 * Unit w vector
 */
const Vector4D Vector4D::UnitW = Vector4D(0.0f, 0.0f, 0.0f, 1.0f);

/*
 * Default constructor
 */
Vector4D::Vector4D()
{
    components[0] = 0.0f;
    components[1] = 0.0f;
    components[2] = 0.0f;
    components[3] = 0.0f;
}

/*
 * Constructor with components given
 */
Vector4D::Vector4D(float x, float y, float z, float w)
{
    components[0] = x;
    components[1] = y;
    components[2] = z;
    components[3] = w;
}

/*
 * Constructor with 2D vector
 */
Vector4D::Vector4D(const Vector2D& vec2d, float z, float w)
{
    components[0] = vec2d[0];
    components[1] = vec2d[1];
    components[2] = z;
    components[3] = w;
}

/*
 * Constructor with 3D vector
 */
Vector4D::Vector4D(const Vector3D& vec3d, float w)
{
    components[0] = vec3d[0];
    components[1] = vec3d[1];
    components[2] = vec3d[2];
    components[3] = w;
}

/*
 * Copy constructor
 */
Vector4D::Vector4D(const Vector4D& other)
{
    for (int i = 0; i < 4; i++)
        components[i] = other.components[i];
}

/*
 * Destructor
 */
Vector4D::~Vector4D()
{
    //Do nothing
}

/*
 * Multiplication operator
 */
Vector4D Vector4D::operator*(float scalar) const
{
    //Component wise multiplication
    return Vector4D(components[0] * scalar,
                    components[1] * scalar,
                    components[2] * scalar,
                    components[3] * scalar);
}

/*
 * Multiplication assignment operator
 */
Vector4D& Vector4D::operator*=(float scalar)
{
    *this = (*this) * scalar;
    return *this;
}

/*
 * Division operator
 */
Vector4D Vector4D::operator/(float scalar) const
{
    //Component wise division
    return Vector4D(components[0] / scalar,
                    components[1] / scalar,
                    components[2] / scalar,
                    components[3] / scalar);
}

/*
 * Division assignment operator
 */
Vector4D& Vector4D::operator/=(float scalar)
{
    *this = (*this) / scalar;
    return *this;
}

/*
 * Addition operator
 */
Vector4D Vector4D::operator+(const Vector4D& other) const
{
    //Component wise addition
    return Vector4D(components[0] + other.components[0],
                    components[1] + other.components[1],
                    components[2] + other.components[2],
                    components[3] + other.components[3]);
}

/*
 * Addition assignment operator
 */
Vector4D& Vector4D::operator+=(const Vector4D& other)
{
    *this = (*this) + other;
    return *this;
}

/*
 * Subtraction operator
 */
Vector4D Vector4D::operator-(const Vector4D& other) const
{
    //Component wise subtraction
    return Vector4D(components[0] - other.components[0],
                    components[1] - other.components[1],
                    components[2] - other.components[2],
                    components[3] - other.components[3]);
}

/*
 * Subtraction assignment operator
 */
Vector4D& Vector4D::operator-=(const Vector4D& other)
{
    *this = *this - other;
    return *this;
}

/*
 * Assigment operator
 */
Vector4D& Vector4D::operator=(const Vector4D& other)
{
    for (int i = 0; i < 4; i++)
        components[i] = other.components[i];

    return *this;
}

/*
 * Equality operator
 */
bool Vector4D::operator==(const Vector4D& other) const
{
    return components[0] == other.components[0] &&
           components[1] == other.components[1] &&
           components[2] == other.components[2] &&
           components[3] == other.components[3];
}

/*
 * Inequality operator
 */
bool Vector4D::operator!=(const Vector4D& other) const
{
    return !((*this) == other);
}

/*
 * Constant indexer
 */
const float& Vector4D::operator[](int index) const
{
    assert(index == 0 || index == 1 || index == 2 || index == 3);

    return components[index];
}

/*
 * Indexer
 */
float& Vector4D::operator[](int index)
{
    assert(index == 0 || index == 1 || index == 2 || index == 3);

    return components[index];
}

/*
 * Dot product
 */
float Vector4D::Dot(const Vector4D& other) const
{
    return components[0] * other.components[0] +
           components[1] * other.components[1] +
           components[2] * other.components[2] +
           components[3] * other.components[3];
}

/*
 * Length
 */
float Vector4D::Length() const
{
    return sqrt(Dot(*this));
}

/*
 * Length squared
 */
float Vector4D::LengthSquared() const
{
    return Dot(*this);
}

/*
 * Distance
 */
float Vector4D::Distance(const Vector4D& other) const
{
    return (*this - other).Length();
}

/*
 * Distance squared
 */
float Vector4D::DistanceSquared(const Vector4D& other) const
{
    return (*this - other).LengthSquared();
}

/*
 * Normalize
 */
Vector4D Vector4D::Normalize() const
{
    return Vector4D(*this) * (1 / this->Length());
}

/*
 * XY
 */
Vector2D Vector4D::XY() const
{
    return Vector2D(components[0], components[1]);
}

/*
 * XYZ
 */
Vector3D Vector4D::XYZ() const
{
    return Vector3D(components[0], components[1], components[2]);
}

/*
 * Ostream operator
 */
std::ostream& operator<<(std::ostream& stream, const Vector4D& v)
{
    return stream << "(" << v[0] << ", " << v[1] << ", " << v[2] << "," << v[3] << ")";
}
