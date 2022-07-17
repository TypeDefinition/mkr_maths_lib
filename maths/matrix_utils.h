#pragma once

#include <optional>
#include "matrix.h"
#include "maths_util.h"
#include "vector3.h"

namespace mkr {
    class matrix_util {
    public:
        matrix_util() = delete;

        /**
         * @brief Generate the homogeneous translation matrix given a translation vector
         * 
         * @param _translation the translation vectors data that will be used
         * @return matrix4x4 the homogeneous translation matrix
         */
        static matrix4x4 translation_matrix(const vector3& _translation);

        /**
         * @brief Generate the homogeneous rotation matrix about the X-axis given angle in radians.
         * 
         * @param _angle angle to rotate about in radians
         * @return matrix4x4 homogeneous rotation matrix about the X-axis given angle in radians.
         */
        static matrix4x4 rotation_matrix_x(float _angle);

        /**
         * @brief Generate the homogeneous rotation matrix about the Y-axis given angle in radians.
         * 
         * @param _angle angle to rotate about in radians
         * @return matrix4x4 homogeneous rotation matrix about the Y-axis given angle in radians.
         */
        static matrix4x4 rotation_matrix_y(float _angle);

        /**
         * @brief Generate the homogeneous rotation matrix about the Z-axis given angle in radians.
         * 
         * @param _angle angle to rotate about in radians
         * @return matrix4x4 homogeneous rotation matrix about the Z-axis given angle in radians.
         */
        static matrix4x4 rotation_matrix_z(float _angle);

        /**
         * @brief Generate the homogeneous rotation matrix about all 3 XYZ-axis 
         * given a vector of euler angles rotations.
         * 
         * @param _euler_angles the euler angles to the XYZ axis
         * @return matrix4x4 homogeneous rotation matrix about the 3 XYZ-axis.
         */
        static matrix4x4 rotation_matrix(const vector3& _euler_angles);

        /**
         * @brief Generate the homogeneous scale matrix given scale vector
         * 
         * @param _scale the vector representating the scale of the object in XYZ
         * @return matrix4x4 
         */
        static matrix4x4 scale_matrix(const vector3& _scale);

        /**
         * @brief Get the model matrix given translation, rotation and scale.
         * 
         * @param _translation translation of the object in 3D space
         * @param _euler_angles rotation of the object in 3D space
         * @param _scale the scale of the object in 3D space
         * @return matrix4x4 the Homogeneous matrix generated
         */
        static matrix4x4 model_matrix(const vector3& _translation, const vector3& _euler_angles, const vector3& _scale);

        /**
         * @brief Get the view matrix given the cameras orientation and position
         * 
         * @param _forward the forward vector
         * @param _up the up vector
         * @param _position the position in 3D space
         * @return matrix4x4 the view matrix in 3D space
         */
        static matrix4x4 view_matrix(const vector3& _forward, const vector3& _up, const vector3& _position);

        matrix4x4 perspective_matrix(float _aspect_ratio, float _fov, float _near_plane, float _far_plane);

        matrix4x4
        orthographic_matrix(float _left, float _right, float _top, float _bottom, float _near_plane, float _far_plane);

        matrix4x4 orthographic_matrix(float _aspect_ratio, float _ortho_size, float _near_plane, float _far_plane);

        /**
         * @brief Get the minor square matrix given a matrix of a bigger size
         * 
         * @tparam Size the size of the original matrix
         * @param _matrix the original matrix
         * @param _cofactor_col the column that will not be used the minor matrix
         * @param _cofactor_row the row that will not be used in the minor matrix
         * @return matrix<Size - 1, Size - 1> the minor square matrix of size - 1
         */
        template<size_t Size>
        static matrix<Size-1, Size-1>
        minor_matrix(const matrix<Size, Size>& _matrix, size_t _cofactor_col, size_t _cofactor_row)
        {
            matrix<Size-1, Size-1> mat;
            for (float major_col = 0, minor_col = 0; major_col<Size; ++major_col) {
                if (major_col==_cofactor_col) continue;
                for (float major_row = 0, minor_row = 0; major_row<Size; ++major_row) {
                    if (major_row==_cofactor_row) continue;
                    mat[minor_col][minor_row] = _matrix[major_col][major_row];
                    ++minor_row;
                }
                ++minor_col;
            }
        }

        /**
         * @brief Get the determinant of given square matrix
         * 
         * @tparam Size the size of the matrix
         * @param _matrix the matrix that will be used
         * @return float the determinant of the matrix
         */
        template<size_t Size>
        static float determinant(const matrix<Size, Size>& _matrix)
        {
            float det = 0.0f;
            for (size_t column = 0; column<Size; ++column) {
                float cofactor = _matrix[column][0];
                //determine sign-ness of that iteration
                cofactor *= 1+((column & 1)*-2);

                matrix<Size-1, Size-1> minor_mat = minor_matrix(_matrix, column, 0);
                det += determinant(minor_mat)*cofactor;
            }

            return det;
        }

        /**
         * @brief Get the determinant of a 2 by 2 matrix
         * 
         * @param _matrix the matrix that will be used
         * @return float the determinant of the matrix
         */
        static inline float determinant(const matrix2x2& _matrix)
        {
            return (_matrix[0][0]*_matrix[1][1])-(_matrix[1][0]*_matrix[0][1]);
        };

        /**
         * @brief Get the determinant of a 1 by 1 matrix
         * 
         * @param _matrix the matrix that will be used
         * @return float the determinant of the matrix
         */
        static inline float determinant(const matrix<1, 1>& _matrix) { return _matrix[0][0]; };

        /**
         * @brief Get the cofactor matrix of a given matrix
         * 
         * @tparam Size the size of the matrix
         * @param _matrix the original matrix
         * @return matrix<Size, Size> the cofactor matrix generated from the original matrix
         */
        template<size_t Size>
        static matrix<Size, Size> cofactor_matrix(const matrix<Size, Size>& _matrix)
        {
            matrix<Size, Size> mat;

            for (size_t column = 0; column<Size; ++column) {
                for (size_t row = 0; row<Size; ++row) {
                    float determinant = determinant(minor_matrix(_matrix, column, row));
                    mat[column][row] = determinant*(1+(column ^ row)*-2);
                }
            }

            return mat;
        }

        /**
         * @brief Get the cofactor matrix of a 1 by 1 matrix
         * 
         * @param _matrix the original matrix
         * @return matrix<1, 1> the cofactor matrix generated from the original matrix
         */
        static inline matrix<1, 1> cofactor_matrix(const matrix<1, 1>& _matrix) { return _matrix; }

        /**
         * @brief Get the adjugate matrix of a given matrix
         * 
         * @tparam Size the size of the original matrix
         * @param _matrix the original matrix
         * @return matrix<Size, Size> the adjugate matrix gernerated from the original matrix
         */
        template<size_t Size>
        static matrix<Size, Size> adjugate_matrix(const matrix<Size, Size>& _matrix)
        {
            return cofactor_matrix(_matrix).transposed();
        }

        /**
         * @brief Get the inverse matrix of given matrix
         * 
         * @tparam Size the size of the original matrix
         * @param _matrix the matrix that will be used to find its inverse
         * @return an optional result depending on whether an inverse exist. 
         * matrix<Size, Size> the inverse of the given square matrix
         */
        template<size_t Size>
        static std::optional<matrix<Size, Size>> inverse_matrix(const matrix<Size, Size>& _matrix)
        {
            if (!has_inverse(_matrix)) return std::nullopt;

            return 1.0f/determinant(_matrix)*adjugate_matrix(_matrix);
        }

        /**
         * @brief Get the inverse matrix of given 4 by 4 matrix
         * 
         * @param _matrix the original 4 by 4 matrix 
         * @return matrix4x4 the resultant inverse
         */
        static matrix4x4 inverse_matrix(const matrix4x4& _matrix);

        /**
         * @brief boolean to test if an inverse exist
         * 
         * @tparam Size the size of the matrix
         * @param _matrix the matrix that will be used to test
         * @return true if the determinant of the matrix is not 0
         * @return false if the determinant of the matrix is 0
         */
        template<size_t Size>
        static bool has_inverse(const matrix<Size, Size>& _matrix)
        {
            return !maths_util::approx_equal(determinant(_matrix), 0.0f);
        }

    };
}