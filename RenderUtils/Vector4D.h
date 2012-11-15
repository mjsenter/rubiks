#ifndef VECTOR4D_H
#define VECTOR4D_H

#include <iostream>
#include "Vector2D.h"
#include "Vector3D.h"

/**
 * \brief Class representing a 4D Vector
 *
 * \author Jonathan Henze
 */
class Vector4D
{
public:

    /**
     * \brief Constructs a Vector4D with all components initialized to 0
     */
    Vector4D();

    /**
     * \brief Constructs a Vector4D with the given components
     *
     * \param[in] x - X component
     * \param[in] y - Y component
     * \param[in] z - Z component
     * \param[in] w - W component
     */
    Vector4D(float x, float y, float z, float w);

    /**
     * \brief Constructs a Vector4D from an existing instance
     *
     * \param[in] other - Vector4D to copy
     */
    Vector4D(const Vector4D& other);

    /**
     * \brief Constructs a Vector4D from a Vector2D
     *
     * \param[in] vec2d - 2D vector to copy
     * \param[in] z     - Z component
     * \param[in] w     - W component
     */
    Vector4D(const Vector2D& vec2d, float z = 0.0f, float w = 0.0f);

    /**
     * \brief Constructs a Vector4D from a Vector3D
     *
     * \param[in] vec3d - 3D vector to copy
     * \param[in] w     - W component
     */
    Vector4D(const Vector3D& vec3d, float w = 0.0f);

    /**
     * \brief Vector4D destructor
     */
    ~Vector4D();

    /**
     * \brief Calculates the dot product between two Vector4Ds
     *
     * \param[in] other - Vector4D to dot product with
     *
     * \return Dot product result
     */
    float Dot(const Vector4D& other) const;

    /**
     * \brief Calculates the length of this Vector4D
     *
     * \return Length
     */
    float Length() const;

    /**
     * \brief Calculates the square of the length of this Vector4D
     *
     * This is faster than calculating the length of the Vector4D
     * and should be used instead if possible.
     *
     * \return The square of the length of this Vector4D
     */
    float LengthSquared() const;

    /**
     * \brief Calculates the distance between this Vector4D and another
     *
     * This will always be a positive quantity or 0 if the vectors are equal.
     *
     * \param[in] other - Vector3D to find distance to
     *
     * \return The distance between this Vector4D and the other
     */
    float Distance(const Vector4D& other) const;

    /**
     * \brief Calculates the square of the distance between this Vector4D and another
     *
     * This is faster than finding the distance between the two vectors
     * and should be used instead if possible.
     *
     * \param[in] other - Vector4D to find distance squared to
     *
     * \return The square of the distance between this Vector4D and the other
     */
    float DistanceSquared(const Vector4D& other) const;

    /**
     * \brief Calculates a normal vector of this Vector4D
     *
     * This Vector4D remains unchanged and a normalized copy is returned.
     *
     * \return A normalized copy of this Vector4D
     */
    Vector4D Normalize() const;

    /**
     * \brief Gets a Vector2D containing the X and Y components of this Vector4D
     *
     * \return Vector2D containing the X and Y components of this Vector4D
     */
    Vector2D XY() const;

    /**
     * \brief Gets a Vector3D containing the X, Y and Z components of this Vector4D
     *
     * \return Vector2D containing the X, Y and Z components of this Vector4D
     */
    Vector3D XYZ() const;

    /**
     * \brief Adds two Vector4Ds
     *
     * \param[in] other - Vector4D to add
     *
     * \return Sum of the two Vector4Ds
     */
    Vector4D operator+(const Vector4D& other) const;

    /**
     * \brief Subtracts two Vector4Ds
     *
     * \param[in] other - Vector4D to subtract
     *
     * \return Difference of the two Vector4Ds
     */
    Vector4D operator-(const Vector4D& other) const;

    /**
     * \brief Gets the negation of this vector
     *
     * \return The negation of this vector
     */
    inline Vector4D operator-() const
    {
        return (*this) * -1.0f;
    }

    /**
     * \brief Multiplies a Vector4D by a scalar
     *
     * \param[in] scalar - Scalar to multiply by
     *
     * \return Result of the multiplication
     */
    Vector4D operator*(float scalar) const;

    /**
     * \brief Divides a Vector4D by a scalar
     *
     * \param[in] scalar - Scalar to divide by
     *
     * \return Result of the division
     */
    Vector4D operator/(float scalar) const;

    /**
     * \brief Assigns this Vector4D to be equal to another instance
     *
     * \param[in] other - Vector4D to copy
     *
     * \return This Vector4D
     */
    Vector4D& operator=(const Vector4D& other);

    /**
     * \brief Adds another Vector4D to this Vector4D
     *
     * \param[in] other - Vector4D to add
     *
     * \return This Vector4D
     */
    Vector4D& operator+=(const Vector4D& other);

    /**
     * \brief Subtracts another Vector4D from this Vector4D
     *
     * \param[in] other - Vector4D to subtract
     *
     * \return This Vector4D
     */
    Vector4D& operator-=(const Vector4D& other);

    /**
     * \brief Multiplies this Vector4D by a scalar
     *
     * \param[in] scalar - Scalar to multiply by
     *
     * \return This Vector4D
     */
    Vector4D& operator*=(float scalar);

    /**
     * \brief Divides this Vector4D by a scalar
     *
     * \param[in] scalar - Scalar to divide by
     *
     * \return This Vector4D
     */
    Vector4D& operator/=(float scalar);

    /**
     * \brief Checks equality between two Vector4Ds
     *
     * \param[in] other - Vector4D to check for equality
     *
     * \return Whether the two Vector4Ds are equal
     */
    bool operator==(const Vector4D& other) const;

    /**
     * \brief Checks inequality between two Vector4Ds
     *
     * \param[in] other - Vector4D to check for inequality
     *
     * \return Whether the two Vector4Ds are not equal
     */
    bool operator!=(const Vector4D& other) const;

    /**
     * \brief Retrieves the a component of this Vector4D
     *
     * \param[in] index - Index of the component where x=0, y=1 and z=2
     *
     * \return The desired component
     */
    float& operator[](int index);

    /**
     * \brief Retrieves the a component of this Vector4D
     *
     * \param[in] index - Index of the component where x=0, y=1 and z=2
     *
     * \return The desired component
     */
    const float& operator[](int index) const;

    /**
     * \brief Constant field representing the zero vector
     */
    static const Vector4D Zero;

    /**
     * \brief Constant field representing the unit x vector
     */
    static const Vector4D UnitX;

    /**
     * \brief Constant field representing the unit y vector
     */
    static const Vector4D UnitY;

    /**
     * \brief Constant field representing the unit z vector
     */
    static const Vector4D UnitZ;

    /**
     * \brief Constant field representing the unit w vector
     */
    static const Vector4D UnitW;
    
private:

    /**
     * \brief The components of the vector
     */
    float components[4];
};

/**
 * \brief Output stream operator
 *
 * \param[in] stream - Stream to output to
 * \param[in] v      - Vector4D to output
 *
 * \return stream
 */
std::ostream& operator<<(std::ostream& stream, const Vector4D& v);

/**
 * \brief Multiplies a Vector4D by a scalar
 *
 * \param[in] scalar - Scalar to multiply by
 * \param[in] v      - Vector4D to multiply
 *
 * \return Result of the multiplication
 */
inline Vector4D operator*(float scalar, const Vector4D& v)
{
    // Call the member function
    return v * scalar;
}

#endif
