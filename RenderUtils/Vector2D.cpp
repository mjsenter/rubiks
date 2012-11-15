#include <cassert>
#include <math.h>
#include "Vector2D.h"
#include "Vector3D.h"

/*
 * Zero vector
 */
const Vector2D Vector2D::Zero = Vector2D(0.0f, 0.0f);

/*
 * Unit x vector
 */
const Vector2D Vector2D::UnitX = Vector2D(1.0f, 0.0f);

/*
 * Unit y vector
 */
const Vector2D Vector2D::UnitY = Vector2D(0.0f, 1.0f);

/*
 * Default constructor
 */
Vector2D::Vector2D()
{
    components[0] = 0.0f;
    components[1] = 0.0f;
}

/*
 * Constructor with given components
 */
Vector2D::Vector2D(float x, float y)
{
    components[0] = x;
    components[1] = y;
}

/*
 * Copy constructor
 */
Vector2D::Vector2D(const Vector2D& other)
{
    components[0] = other.components[0];
    components[1] = other.components[1];
}

/*
 * Destructor
 */
Vector2D::~Vector2D()
{
    //Do nothing
}

/*
 * Multiplication operator
 */
Vector2D Vector2D::operator*(float scalar) const
{
    return Vector2D(components[0] * scalar, components[1] * scalar);
}

/*
 * Multiplication assignment operator
 */
Vector2D& Vector2D::operator*=(float scalar)
{
    *this = (*this) * scalar;
    return *this;
}

/*
 * Division operator
 */
Vector2D Vector2D::operator/(float scalar) const
{
    return Vector2D(components[0] / scalar, components[1] / scalar);
}

/*
 * Division assignment operator
 */
Vector2D& Vector2D::operator/=(float scalar)
{
    *this = (*this) / scalar;
    return *this;
}

/*
 * Addition operator
 */
Vector2D Vector2D::operator+(const Vector2D& other) const
{
    return Vector2D(components[0] + other.components[0],
                    components[1] + other.components[1]);
}

/*
 * Addition assignment operator
 */
Vector2D& Vector2D::operator+=(const Vector2D& other)
{
    *this = (*this) + other;
    return *this;
}

/*
 * Subtraction operator
 */
Vector2D Vector2D::operator-(const Vector2D& other) const
{
    return Vector2D(components[0] - other.components[0],
                    components[1] - other.components[1]);
}

/*
 * Subtraction assignment operator
 */
Vector2D& Vector2D::operator-=(const Vector2D& other)
{
    *this = *this - other;
    return *this;
}

/*
 * Assignment operator
 */
Vector2D& Vector2D::operator=(const Vector2D& other)
{
    components[0] = other.components[0];
    components[1] = other.components[1];

    return *this;
}

/*
 * Equality operator
 */
bool Vector2D::operator==(const Vector2D& other) const
{
    return components[0] == other.components[0] &&
           components[1] == other.components[1];
}

/*
 * Inequality operator
 */
bool Vector2D::operator!=(const Vector2D& other) const
{
    return !((*this) == other);
}

/*
 * Indexer
 */
float& Vector2D::operator[](int index)
{
    assert(index == 0 || index == 1);

    return components[index];
}

/**
 * Constant indexer
 */
const float& Vector2D::operator[](int index) const
{
    assert(index == 0 || index == 1);

    return components[index];
}

/*
 * Dot product
 */
float Vector2D::Dot(const Vector2D& other) const
{
    return components[0] * other.components[0] +
           components[1] * other.components[1];
}

/*
 * Cross product
 */
float Vector2D::Cross(const Vector2D& other) const
{
    //Promote to vector3Ds with no z, perform the cross product
    //and then return the z component
    return Vector3D(*this, 0).Cross(Vector3D(other, 0))[2];
}

/*
 * Length
 */
float Vector2D::Length() const
{
    return sqrt(Dot(*this));
}

/*
 * Length squared
 */
float Vector2D::LengthSquared() const
{
    return Dot(*this);
}

/*
 * Distance
 */
float Vector2D::Distance(const Vector2D& other) const
{
    return (*this - other).Length();
}

/*
 * Distance squared
 */
float Vector2D::DistanceSquared(const Vector2D& other) const
{
    return (*this - other).LengthSquared();
}

/*
 * Normalize
 */
Vector2D Vector2D::Normalize() const
{
    return (*this) / this->Length();
}

/*
 * Ostream operator
 */
std::ostream& operator<<(std::ostream& stream, const Vector2D& v)
{
    return stream << "(" << v[0] << ", " << v[1] << ")";
}
