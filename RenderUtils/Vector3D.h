#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>
#include "Vector2D.h"

/**
 * \brief Class representing a 3D Vector
 *
 * \author Jonathan Henze
 */
class Vector3D
{
public:

    /**
     * \brief Constructs a Vector3D with all components initialized to 0
     */
    Vector3D();

    /**
     * \brief Constructs a Vector3D with the given components
     *
     * \param[in] x - X component
     * \param[in] y - Y component
     * \param[in] z - Z component
     */
    Vector3D(float x, float y, float z);

    /**
     * \brief Constructs a Vector3D from an existing instance
     *
     * \param[in] other - Vector3D to copy
     */
    Vector3D(const Vector3D& other);

    /**
     * \brief Constructs a Vector3D from a Vector2D
     *
     * \param[in] vec2d - 2D vector to copy
     * \param[in] z     - Z component
     */
    Vector3D(const Vector2D& vec2d, float z = 0.0f);

    /**
     * \brief Vector3D destructor
     */
    ~Vector3D();

    /**
     * \brief Calculates the dot product between two Vector3Ds
     *
     * \param[in] other - Vector3D to dot product with
     *
     * \return Dot product result
     */
    float Dot(const Vector3D& other) const;

    /**
     * \brief Calculates the length of this Vector3D
     *
     * \return Length
     */
    float Length() const;

    /**
     * \brief Calculates the square of the length of this Vector3D
     *
     * This is faster than calculating the length of the Vector3D
     * and should be used instead if possible.
     *
     * \return The square of the length of this Vector3D
     */
    float LengthSquared() const;

    /**
     * \brief Calculates the distance between this Vector3D and another
     *
     * This will always be a positive quantity or 0 if the vectors are equal.
     *
     * \param[in] other - Vector3D to find distance to
     *
     * \return The distance between this Vector3D and the other
     */
    float Distance(const Vector3D& other) const;

    /**
     * \brief Calculates the square of the distance between this Vector3D and another
     *
     * This is faster than finding the distance between the two vectors
     * and should be used instead if possible.
     *
     * \param[in] other - Vector3D to find distance squared to
     *
     * \return The square of the distance between this Vector3D and the other
     */
    float DistanceSquared(const Vector3D& other) const;

    /**
     * \brief Calculates the cross product between two Vector3Ds
     *
     * \param[in] other - Vector3D to cross product with
     *
     * \return Cross product result
     */
    Vector3D Cross(const Vector3D& other) const;

    /**
     * \brief Calculates a normal vector of this Vector3D
     *
     * This Vector3D remains unchanged and a normalized copy is returned.
     *
     * \return A normalized copy of this Vector3D
     */
    Vector3D Normalize() const;

    /**
     * \brief Gets a Vector2D containing the X and Y components of this Vector3D
     *
     * \return Vector2D containing the X and Y components of this Vector3D
     */
    Vector2D XY() const;

    /**
     * \brief Adds two Vector3Ds
     *
     * \param[in] other - Vector3D to add
     *
     * \return Sum of the two Vector3Ds
     */
    Vector3D operator+(const Vector3D& other) const;

    /**
     * \brief Subtracts two Vector3Ds
     *
     * \param[in] other - Vector3D to subtract
     *
     * \return Difference of the two Vector3Ds
     */
    Vector3D operator-(const Vector3D& other) const;

    /**
     * \brief Gets the negation of this vector
     *
     * \return The negation of this vector
     */
    inline Vector3D operator-() const
    {
        return (*this) * -1.0f;
    }

    /**
     * \brief Multiplies a Vector3D by a scalar
     *
     * \param[in] scalar - Scalar to multiply by
     *
     * \return Result of the multiplication
     */
    Vector3D operator*(float scalar) const;

    /**
     * \brief Divides a Vector3D by a scalar
     *
     * \param[in] scalar - Scalar to divide by
     *
     * \return Result of the division
     */
    Vector3D operator/(float scalar) const;

    /**
     * \brief Assigns this Vector3D to be equal to another instance
     *
     * \param[in] other - Vector3D to copy
     *
     * \return This Vector3D
     */
    Vector3D& operator=(const Vector3D& other);

    /**
     * \brief Adds another Vector3D to this Vector3D
     *
     * \param[in] other - Vector3D to add
     *
     * \return This Vector3D
     */
    Vector3D& operator+=(const Vector3D& other);

    /**
     * \brief Subtracts another Vector3D from this Vector3D
     *
     * \param[in] other - Vector3D to subtract
     *
     * \return This Vector3D
     */
    Vector3D& operator-=(const Vector3D& other);

    /**
     * \brief Multiplies this Vector3D by a scalar
     *
     * \param[in] scalar - Scalar to multiply by
     *
     * \return This Vector3D
     */
    Vector3D& operator*=(float scalar);

    /**
     * \brief Divides this Vector3D by a scalar
     *
     * \param[in] scalar - Scalar to divide by
     *
     * \return This Vector3D
     */
    Vector3D& operator/=(float scalar);

    /**
     * \brief Checks equality between two Vector3Ds
     *
     * \param[in] other - Vector3D to check for equality
     *
     * \return Whether the two Vector3Ds are equal
     */
    bool operator==(const Vector3D& other) const;

    /**
     * \brief Checks inequality between two Vector3Ds
     *
     * \param[in] other - Vector3D to check for inequality
     *
     * \return Whether the two Vector3Ds are not equal
     */
    bool operator!=(const Vector3D& other) const;

    /**
     * \brief Retrieves the a component of this Vector3D
     *
     * \param[in] index - Index of the component where x=0, y=1 and z=2
     *
     * \return The desired component
     */
    float& operator[](int index);

    /**
     * \brief Retrieves the a component of this Vector3D
     *
     * \param[in] index - Index of the component where x=0, y=1 and z=2
     *
     * \return The desired component
     */
    const float& operator[](int index) const;

    /**
     * \brief Constant field representing the zero vector
     */
    static const Vector3D Zero;

    /**
     * \brief Constant field representing the unit x vector
     */
    static const Vector3D UnitX;

    /**
     * \brief Constant field representing the unit y vector
     */
    static const Vector3D UnitY;

    /**
     * \brief Constant field representing the unit z vector
     */
    static const Vector3D UnitZ;
    
private:

    /**
     * \brief The components of the vector
     */
    float components[3];
};

/**
 * \brief Output stream operator
 *
 * \param[in] stream - Stream to output to
 * \param[in] v      - Vector3D to output
 *
 * \return stream
 */
std::ostream& operator<<(std::ostream& stream, const Vector3D& v);

/**
 * \brief Multiplies a Vector3D by a scalar
 *
 * \param[in] scalar - Scalar to multiply by
 * \param[in] v      - Vector3D to multiply
 *
 * \return Result of the multiplication
 */
inline Vector3D operator*(float scalar, const Vector3D& v)
{
    // Call the member function
    return v * scalar;
}

#endif
