#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>

/**
 * \brief Class representing a 2D vector
 *
 * \author Jonathan Henze
 */
class Vector2D
{
public:

    /**
     * \brief Constructs a Vector2D with its components initialized to 0
     */
    Vector2D();

    /**
     * \brief Constructs a Vector2D with the given components
     *
     * \param[in] x - X component
     * \param[in] y - Y component
     */
    Vector2D(float x, float y);

    /**
     * \brief Constructs a Vector2D from an existing instance
     *
     * \param[in] other - Vector2D to copy
     */
    Vector2D(const Vector2D& other);

    /**
     * \brief Vector2D destructor
     */
    ~Vector2D();

    /**
     * \brief Calculates the dot product between two Vector2Ds
     *
     * \param[in] other - Vector2D to dot product with
     *
     * \return Dot product result
     */
    float Dot(const Vector2D& other) const;

    /**
     * \brief Calculates the cross product between two Vector2Ds
     *
     * Technically, the cross product is not defined for 2D vectors.
     * This will promote the 2D vectors to 3D vectors, where the cross
     * product is defined, and return the length of the resulting vector.
     *
     * \param[in] other - Vector2D to cross product with
     *
     * \return Cross product result
     */
    float Cross(const Vector2D& other) const;

    /**
     * \brief Calculates the length of this Vector2D
     *
     * \return The length of this Vector2D
     */
    float Length() const;

    /**
     * \brief Calculates the square of the length of this Vector2D
     *
     * This is faster than calculating the length of the Vector2D
     * and should be used instead if possible.
     *
     * \return The square of the length of this Vector2D
     */
    float LengthSquared() const;

    /**
     * \brief Calculates the distance between this Vector2D and another
     *
     * This will always be a positive quantity or 0 if the vectors are equal.
     *
     * \param[in] other - Vector2D to find distance to
     *
     * \return The distance between this Vector2D and the other
     */
    float Distance(const Vector2D& other) const;

    /**
     * \brief Calculates the square of the distance between this Vector2D and another
     *
     * This is faster than finding the distance between the two vectors
     * and should be used instead if possible.
     *
     * \param[in] other - Vector2D to find distance squared to
     *
     * \return The square of the distance between this Vector2D and the other
     */
    float DistanceSquared(const Vector2D& other) const;

    /**
     * \brief Calculates a normal vector in the direction of this Vector2D
     *
     * \return A normal vector in the direction of this Vector2D
     */
    Vector2D Normalize() const;

    /**
     * \brief Adds two Vector2Ds together
     *
     * \param[in] other - Vector2D to add
     *
     * \return The sum of the Vector2Ds
     */
    Vector2D operator+(const Vector2D& other) const;

    /**
     * \brief Subtracts two Vector2Ds
     *
     * \param[in] other - Vector2D to subtract
     *
     * \return The difference of the Vector2Ds
     */
    Vector2D operator-(const Vector2D& other) const;

    /**
     * \brief Gets the negation of this vector
     *
     * \return The negation of this vector
     */
    inline Vector2D operator-() const
    {
        return (*this) * -1.0f;
    }

    /**
     * \brief Multiplies a Vector2D by a scalar
     *
     * \param[in] scalar - Scalar to multiply by
     *
     * \return Result of the multiplication
     */
    Vector2D operator*(float scalar) const;

    /**
     * \brief Divides a Vector2D by a scalar
     *
     * \param[in] scalar - Scalar to divide by
     *
     * \return Result of the division
     */
    Vector2D operator/(float scalar) const;

    /**
     * \brief Assigns this Vector2D to be equal to another instance
     *
     * \param[in] other - Vector2D to copy
     *
     * \return This Vector2D
     */
    Vector2D& operator=(const Vector2D& other);

    /**
     * \brief Adds another Vector2D to this one
     *
     * \param[in] other - Vector2D to add
     *
     * \return This Vector2D
     */
    Vector2D& operator+=(const Vector2D& other);

    /**
     * \brief Subtracts another Vector2D from this one
     *
     * \param[in] other - Vector2D to subtract
     *
     * \return This Vector2D
     */
    Vector2D& operator-=(const Vector2D& other);

    /**
     * \brief Multiplies this Vector2D by a scalar
     *
     * \param[in] scalar - Scalar to multiply by
     *
     * \return This Vector2D
     */
    Vector2D& operator*=(float scalar);

    /**
     * \brief Divides this Vector2D by a scalar
     *
     * \param[in] scalar - Scalar to divide by
     *
     * \return This Vector2D
     */
    Vector2D& operator/=(float scalar);

    /**
     * \brief Checks equality between two Vector2Ds
     *
     * \param[in] other - Vector2D to check equality against
     *
     * \return Whether the two Vector2Ds are equal
     */
    bool operator==(const Vector2D& other) const;

    /**
     * \brief Checks inequality between two Vector2Ds
     *
     * \param[in] other - Vector2D to check inequality against
     *
     * \return Whether the two Vector2Ds are not equal
     */
    bool operator!=(const Vector2D& other) const;

    /**
     * \brief Retrieves a component of this Vector2D
     *
     * \param[in] index - Index of the component where x=0 and y=1
     *
     * \return Component of this Vector2D
     */
    float& operator[](int index);

    /**
     * \brief Retrieves a component of this Vector2D
     *
     * \param[in] index - Index of the component where x=0 and y=1
     *
     * \return Component of this Vector2D
     */
    const float& operator[](int index) const;

    /**
     * \brief Constant field containing the zero vector
     */
    static const Vector2D Zero;

    /**
     * \brief Constant field containing the unit x vector
     */
    static const Vector2D UnitX;

    /**
     * \brief Constant field containing the unit y vector
     */
    static const Vector2D UnitY;
    
private:

    /**
     * \brief Components of this vector
     */
    float components[2];
};

/**
 * \brief Output stream operator
 *
 * \param[in] stream - Stream to output to
 * \param[in] v      - Vector2D to output
 *
 * \return stream
 */
std::ostream& operator<<(std::ostream& stream, const Vector2D& v);

/**
 * \brief Multiplies a Vector2D by a scalar
 *
 * \param[in] scalar - Scalar to multiply by
 * \param[in] v      - Vector2D to multiply
 *
 * \return Result of the multiplication
 */
inline Vector2D operator*(float scalar, const Vector2D& v)
{
    // Call the member function
    return v * scalar;
}

#endif
