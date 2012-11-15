#ifndef MATRIX_H
#define MATRIX_H

#include "Vector3D.h"
#include "Vector4D.h"

/**
 * \brief Class representing a 4x4 matrix
 *
 * \author Jonathan Henze
 */
class Matrix
{
public:

    /**
     * \brief Creates a 4x4 matrix with all values initialized to 0
     */
    Matrix();

    /**
     * \brief Creates a matrix initialized to the values from an existing matrix
     *
     * \param[in] other - Matrix to copy
     */
    Matrix(const Matrix& other);

    /**
     * \brief Creates a matrix initialized with the given values
     *
     * \param[in] row#col# - The value for that position in the matrix
     */
    Matrix(float row0col0, float row0col1, float row0col2, float row0col3,
           float row1col0, float row1col1, float row1col2, float row1col3,
           float row2col0, float row2col1, float row2col2, float row2col3,
           float row3col0, float row3col1, float row3col2, float row3col3);

    /**
     * \brief Creates a matrix initialized to the values from an existing array
     *
     * \param[in] data - Array to copy from, must have at least 16 elements
     */
    Matrix(const float* data);

    /**
     * \brief Matrix destructor
     */
    ~Matrix();

    /**
     * \brief Computes the inverse of the matrix
     *
     * \return The inverse of this matrix
     */
    Matrix Inverse() const;

    /**
     * \brief Computes the transpose of the matrix
     *
     * \return The transpose of this matrix
     */
    Matrix Transpose() const;

    /**
     * \brief Computes the determinant of the matrix
     *
     * \return The determinant of the matrix
     */
    float Determinant() const;

    /**
     * \brief Transforms a Vector3D by this matrix
     *
     * \param[in] vec3d - Vector3D to transform
     *
     * \return Transformed copy of the input vector
     */
    Vector3D Transform(const Vector3D& vec3d) const;

    /**
     * \brief Transforms a Vector3D representing a normal vector by this matrix
     *
     * \param[in] vec3d - Vector3D representing a normal to transform
     *
     * \return Transformed copy of the input vector, which is also normal
     */
    Vector3D TransformNormal(const Vector3D& vec3d) const;

    /**
     * \brief Multiplies a Vector4D by this matrix
     *
     * \param[in] vec4d - Vector4D to be multiplied
     *
     * \return Multiplied copy of the vector
     */
    Vector4D operator*(const Vector4D& vec4d) const;

    /**
     * \brief Multiplies the matrix by a scalar
     *
     * \param[in] scalar - Scalar to multiply by
     *
     * \return Scaled matrix
     */
    Matrix operator*(float scalar) const;

    /**
     * \brief Adds two matrices together
     *
     * \param[in] other - Matrix to add
     *
     * \return Sum of the two matrices
     */
    Matrix operator+(const Matrix& other) const;

    /**
     * \brief Subtracts two matrices
     *
     * \param[in] other - Matrix to subtract
     *
     * \return Difference of the two matrices
     */
    Matrix operator-(const Matrix& other) const;

    /**
     * \brief Multiplies two matrices together
     *
     * \param[in] other - Matrix to multiply
     *
     * \return Product of the two matrices
     */
    Matrix operator*(const Matrix& other) const;

    /**
     * \brief Assigns this matrix to be equal to an existing matrix
     *
     * \param[in] other - Matrix to copy
     *
     * \return This matrix
     */
    Matrix& operator=(const Matrix& other);

    /**
     * \brief Multiplies the matrix by a scalar
     *
     * \param[in] scalar - Scalar to multiply by
     *
     * \return This matrix
     */
    Matrix& operator*=(float scalar);

    /**
     * \brief Adds a matrix to this matrix
     *
     * \param[in] other - Matrix to add
     *
     * \return This matrix
     */
    Matrix& operator+=(const Matrix& other);

    /**
     * \brief Subtracts a matrix from this matrix
     *
     * \param[in] other - Matrix to subtract
     *
     * \return This matrix
     */
    Matrix& operator-=(const Matrix& other);

    /**
     * \brief Retrieves the value at the given position in the array
     *
     * \param[in] row - Row of the element in the matrix
     * \param[in] col - Column of the element in the matrix
     *
     * \return Value at the given row and column
     */
    inline float& operator()(int row, int col);
    inline const float& operator()(int row, int col) const;

    /**
     * \brief The identity matrix
     */
    static const Matrix Identity;

    /**
     * \brief Creates a matrix that will translate points
     *
     * \param[in] vec3d - Translation vector
     *
     * \return Translation matrix
     */
    static Matrix GetTranslate(const Vector3D& vec3d);

    /**
     * \brief Creates a matrix that will translate points
     *
     * \param[in] x - X parameter to translate by
     * \param[in] y - Y parameter to translate by
     * \param[in] z - Z parameter to translate by
     *
     * \return Translation matrix
     */
    static Matrix GetTranslate(float x, float y, float z);

    /**
     * \brief Gets a matrix to rotate points about the x-axis
     *
     * \param[in] degrees - Degrees to rotate around the x-axis
     *
     * \return Rotation matrix
     */
    static Matrix GetRotateX(float degrees);

    /*
     * \brief Gets a matrix to rotate points about the y-axis
     *
     * \param[in] degrees - Degrees to rotate around the y-axis
     *
     * \return Rotation matrix
     */
    static Matrix GetRotateY(float degrees);

    /*
     * \brief Gets a matrix to rotate points about the z-axis
     *
     * \param[in] degrees - Degrees to rotate around the z-axis
     *
     * \return Rotation matrix
     */
    static Matrix GetRotateZ(float degrees);

    /**
     * \brief Gets a matrix to rotate points around an axis
     *
     * \param[in] axis    - Unit vector giving the axis of rotation
     * \param[in] degrees - Degrees to rotate around the axis using right hand rule
     *
     * \return Rotation matrix
     */
    static Matrix GetRotate(const Vector3D& axis, float degrees);

    /**
     * \brief Gets a scale matrix that scales points uniformly from the origin
     *
     * \param[in] scale - Uniform scaling factor
     *
     * \return Scale matrix
     */
    static Matrix GetScale(float scale);

    /**
     * \brief Gets a scale matrix that scales points from the origin
     *
     * \param[in] scale - Component wise scaling factors
     *
     * \return Scale matrix
     */
    static Matrix GetScale(const Vector3D& scale);

    /**
     * \brief Gets a scale matrix that scales points from the origin
     *
     * \param[in] x - Scaling factor in the x direction
     * \param[in] y - Scaling factor in the y direction
     * \param[in] z - Scaling factor in the z direction
     *
     * \return Scale matrix
     */
    static Matrix GetScale(float x, float y, float z);

    /**
     * \brief Gets a projection matrix
     *
     * \param[in] near        - Near clipping plane, must be positive
     * \param[in] far         - Far clipping plane, must be > near
     * \param[in] fovY        - Vertical field of view in degrees
     * \param[in] aspectRatio - Aspect ratio defined as width / height
     *
     * \return Projection matrix
     */
    static Matrix GetProjection(float near, float far, float fovY, float aspectRatio);

    /**
     * \brief Gets a view matrix to convert points from world to eye space
     *
     * \param[in] from - World position of the camera
     * \param[in] at   - A point in world space the camera is looking at
     * \param[in] up   - Direction that is considered up for the camera,
     *                   cannot be parallel to the camera's forward direction
     *
     * \return View matrix
     */
    static Matrix GetView(const Vector3D& from, 
                          const Vector3D& at, 
                          const Vector3D& up);

    /**
     * \brief Gets a world matrix to convert points from model to world space
     *
     * \param[in] position - World position of the object
     * \param[in] heading  - Unit vector in the direction the object is facing
     * \param[in] up       - Unit vector in the direction directly above the object,
     *                       must be perpendicular to heading
     *
     * \return World matrix
     */
    static Matrix GetWorld(const Vector3D& position,
                           const Vector3D& heading,
                           const Vector3D& up);

    /**
     * \brief Gets an orthographic projection matrix, useful for 2D rendering
     *
     * Orthographic projections convert points from the range [minXYZ, maxXYZ]
     * to [(-1,-1,-1),(1,1,1)] for rendering.
     *
     * \param[in] minX - Minimum visible x value
     * \param[in] minY - Minimum visible y value
     * \param[in] minZ - Minimum visible z value
     * \param[in] maxX - Maximum visible x value
     * \param[in] maxY - Maximum visible y value
     * \param[in] maxZ - Maximum visible z value
     *
     * \return Othrographic matrix
     */
    static Matrix GetOrtho(float minX, float minY, float minZ,
                           float maxX, float maxY, float maxZ);

    /**
     * \brief Gets an orthographic projection matrix, useful for 2D rendering
     *
     * Orthographic projections convert points from the range [minXYZ, maxXYZ]
     * to [(-1,-1,-1),(1,1,1)] for rendering.
     *
     * \param[in] minXYZ - Minimum visible x, y and z values
     * \param[in] maxXYZ - Maximum visible x, y and z values
     *
     * \return Othrographic matrix
     */
    static Matrix GetOrtho(const Vector3D& minXYZ, const Vector3D& maxXYZ);

private:

    /**
     * \brief Values in the matrix
     */
    float data[4 * 4];
};

/**
 * \brief Multiplies a matrix by a scalar
 *
 * \param[in] scalar - Scalar to multiply by
 * \param[in] matrix - Matrix to multiply
 *
 * \return Scaled matrix
 */
inline Matrix operator*(float scalar, const Matrix& matrix)
{
    return matrix * scalar;
}

#endif
