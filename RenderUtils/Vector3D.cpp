#include <cassert>
#include <math.h>
#include "Vector3D.h"

/*
 * Zero vector
 */
const Vector3D Vector3D::Zero(0.0f, 0.0f, 0.0f);

/*
 * Unit x vector
 */
const Vector3D Vector3D::UnitX(1.0f, 0.0f, 0.0f);

/*
 * Unit y vector
 */
const Vector3D Vector3D::UnitY(0.0f, 1.0f, 0.0f);

/*
 * Unit z vector
 */
const Vector3D Vector3D::UnitZ(0.0f, 0.0f, 1.0f);

/*
 * Default constructor
 */
Vector3D::Vector3D()
{
    components[0] = 0.0f;
    components[1] = 0.0f;
    components[2] = 0.0f;
}

/*
 * Constructor with components given
 */
Vector3D::Vector3D(float x, float y, float z)
{
    components[0] = x;
    components[1] = y;
    components[2] = z;
}

/*
 * Constructor with 2D vector
 */
Vector3D::Vector3D(const Vector2D& vec2d, float z)
{
    components[0] = vec2d[0];
    components[1] = vec2d[1];
    components[2] = z;
}

/*
 * Copy constructor
 */
Vector3D::Vector3D(const Vector3D& other)
{
    for (int i = 0; i < 3; i++)
        components[i] = other.components[i];
}

/*
 * Destructor
 */
Vector3D::~Vector3D()
{
    //Do nothing
}

/*
 * Multiplication operator
 */
Vector3D Vector3D::operator*(float scalar) const
{
    //Component wise multiplication
    return Vector3D(components[0] * scalar,
                    components[1] * scalar,
                    components[2] * scalar);
}

/*
 * Multiplication assignment operator
 */
Vector3D& Vector3D::operator*=(float scalar)
{
    *this = (*this) * scalar;
    return *this;
}

/*
 * Division operator
 */
Vector3D Vector3D::operator/(float scalar) const
{
    //Component wise division
    return Vector3D(components[0] / scalar,
                    components[1] / scalar,
                    components[2] / scalar);
}

/*
 * Division assignment operator
 */
Vector3D& Vector3D::operator/=(float scalar)
{
    *this = (*this) / scalar;
    return *this;
}

/*
 * Addition operator
 */
Vector3D Vector3D::operator+(const Vector3D& other) const
{
    //Component wise addition
    return Vector3D(components[0] + other.components[0],
                    components[1] + other.components[1],
                    components[2] + other.components[2]);
}

/*
 * Addition assignment operator
 */
Vector3D& Vector3D::operator+=(const Vector3D& other)
{
    *this = (*this) + other;
    return *this;
}

/*
 * Subtraction operator
 */
Vector3D Vector3D::operator-(const Vector3D& other) const
{
    //Component wise subtraction
    return Vector3D(components[0] - other.components[0],
                    components[1] - other.components[1],
                    components[2] - other.components[2]);
}

/*
 * Subtraction assignment operator
 */
Vector3D& Vector3D::operator-=(const Vector3D& other)
{
    *this = *this - other;
    return *this;
}

/*
 * Assigment operator
 */
Vector3D& Vector3D::operator=(const Vector3D& other)
{
    for (int i = 0; i < 3; i++)
        components[i] = other.components[i];

    return *this;
}

/*
 * Equality operator
 */
bool Vector3D::operator==(const Vector3D& other) const
{
    return components[0] == other.components[0] &&
           components[1] == other.components[1] &&
           components[2] == other.components[2];
}

/*
 * Inequality operator
 */
bool Vector3D::operator!=(const Vector3D& other) const
{
    return !((*this) == other);
}

/*
 * Constant indexer
 */
const float& Vector3D::operator[](int index) const
{
    assert(index == 0 || index == 1 || index == 2);

    return components[index];
}

/*
 * Indexer
 */
float& Vector3D::operator[](int index)
{
    assert(index == 0 || index == 1 || index == 2);

    return components[index];
}

/*
 * Dot product
 */
float Vector3D::Dot(const Vector3D& other) const
{
    return components[0] * other.components[0] +
           components[1] * other.components[1] +
           components[2] * other.components[2];
}

/*
 * Length
 */
float Vector3D::Length() const
{
    return sqrt(Dot(*this));
}

/*
 * Length squared
 */
float Vector3D::LengthSquared() const
{
    return Dot(*this);
}

/*
 * Distance
 */
float Vector3D::Distance(const Vector3D& other) const
{
    return (*this - other).Length();
}

/*
 * Distance squared
 */
float Vector3D::DistanceSquared(const Vector3D& other) const
{
    return (*this - other).LengthSquared();
}

/*
 * Cross product
 */
Vector3D Vector3D::Cross(const Vector3D& other) const
{
    return Vector3D(components[1]*other.components[2] - components[2]*other.components[1],
                    components[2]*other.components[0] - components[0]*other.components[2],
                    components[0]*other.components[1] - components[1]*other.components[0]);
}

/*
 * Normalize
 */
Vector3D Vector3D::Normalize() const
{
    return Vector3D(*this) * (1 / this->Length());
}

/*
 * XY
 */
Vector2D Vector3D::XY() const
{
    return Vector2D(components[0], components[1]);
}

/*
 * Ostream operator
 */
std::ostream& operator<<(std::ostream& stream, const Vector3D& v)
{
    return stream << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")";
}
