#pragma once

#include <iostream>
#include <cstring>
#include <cstdio>
#include <iomanip>
#include "maths_util.h"

namespace mkr {
    template<size_t Columns, size_t Rows>
    class matrix {
    private:
        float values_[Columns*Rows] = {0.0f};

    public:
        matrix() = default;

        static constexpr bool is_square_matrix = (Columns==Rows);
        static constexpr size_t num_elements = Columns*Rows;
        // static const matrix zero = matrix{};

        /**
         * Returns a zero matrix.
         * @return A zero matrix.
         */
        static constexpr matrix zero() { return matrix{}; }

        /**
         * Returns an identity matrix.
         * @return An identity matrix.
         * @warning This function is only defined for square matrices.
         */
        static constexpr matrix identity() requires is_square_matrix
        {
            matrix result;
            for (size_t i = 0; i<Columns; ++i) {
                result[i][i] = 1.0f;
            }
            return result;
        }

        inline const float* operator[](size_t _column) const { return &values_[_column*Rows]; }

        inline float* operator[](size_t _column) { return &values_[_column*Rows]; }

        bool operator==(const matrix& _rhs) const
        {
            for (size_t i = 0; i<num_elements; i++) {
                if (!maths_util::approx_equal(values_[i], _rhs.values_[i])) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const matrix& _rhs) const { return !(*this==_rhs); }

        matrix operator+(const matrix& _rhs) const
        {
            matrix result;
            for (size_t i = 0; i<num_elements; ++i) {
                result.values_[i] = values_[i]+_rhs.values_[i];
            }
            return result;
        }

        matrix& operator+=(const matrix& _rhs)
        {
            *this = (*this)+_rhs;
            return *this;
        }

        matrix operator-(const matrix& _rhs) const
        {
            matrix result;
            for (size_t i = 0; i<num_elements; ++i) {
                result.values_[i] = values_[i]-_rhs.values_[i];
            }
            return result;
        }

        matrix& operator-=(const matrix& _rhs)
        {
            *this = (*this)-_rhs;
            return *this;
        }

        matrix operator*(float _scalar) const
        {
            matrix result;
            for (size_t i = 0; i<num_elements; ++i) {
                result.values_[i] = values_[i]*_scalar;
            }
            return result;
        }

        matrix& operator*=(float _scalar)
        {
            *this = (*this)*_scalar;
            return *this;
        }

        template<size_t RHSColumns>
        matrix<RHSColumns, Rows> operator*(const matrix<RHSColumns, Columns>& _rhs) const
        {
            matrix<RHSColumns, Rows> result;
            for (size_t i = 0; i<RHSColumns; ++i) {
                for (size_t j = 0; j<Rows; ++j) {
                    for (size_t k = 0; k<Columns; ++k) {
                        result[i][j] += (*this)[k][j]*_rhs[i][k];
                    }
                }
            }
            return result;
        }

        matrix& operator*=(const matrix& _rhs) requires is_square_matrix
        {
            *this = (*this)*_rhs;
            return *this;
        }

        /**
         * @brief Make this matrix a zero matrix.
         */
        inline void set_to_zero() { std::memset(values_, 0, sizeof(values_)); }

        /**
         * @brief Make this matrix an identity matrix.
         * @warning This function is only defined for square matrices.
         */
        inline void set_to_identity() requires is_square_matrix { (*this) = identity(); }

        /**
         * @brief Transpose this matrix.
         */
        inline void transpose() requires is_square_matrix { (*this) = transposed(); }

        /**
         * @brief Returns the transpose of this matrix.
         * @return The transpose of this matrix.
         */
        matrix<Rows, Columns> transposed()
        {
            matrix<Rows, Columns> result;
            for (size_t i = 0; i<Columns; ++i) {
                for (size_t j = 0; j<Rows; ++j) {
                    result[j][i] = (*this)[i][j];
                }
            }
            return result;
        }

        /**
         * @brief Returns a string representation of this matrix.
         * 
         * @param _precision The number of decimals to show for each element
         * @param _padding The padding for each element so that it is printed aligned
         * @return A string representation of this matrix.
         */
        [[nodiscard]] std::string to_string(const int _precision = 1, const int _padding = 5) const
        {
            std::ostringstream out;
            out.precision(_precision);
            out << std::fixed;

            for (size_t i = 0; i<Rows; ++i) {
                for (size_t j = 0; j<Columns; ++j) {
                    out
                            << std::setw(_precision+_padding)
                            << (*this)[j][i]
                            << ",";
                }
                out << '\n';
            }

            return out.str();
        }

        friend std::ostream& operator<<(std::ostream& stream, const matrix<Columns, Rows>& mat)
        {
            stream << "["
                   << std::to_string(Columns)
                   << "X" << std::to_string(Rows) << "]"
                   << "Matrix:\n" << mat.to_string();
            return stream;
        }
    };

    template<size_t Rows>
    class matrix<0, Rows>;
    template<size_t Columns>
    class matrix<Columns, 0>;

    typedef matrix<1, 1> matrix1x1;
    typedef matrix<1, 2> matrix1x2;
    typedef matrix<1, 3> matrix1x3;
    typedef matrix<1, 4> matrix1x4;

    typedef matrix<2, 1> matrix2x1;
    typedef matrix<2, 2> matrix2x2;
    typedef matrix<2, 3> matrix2x3;
    typedef matrix<2, 4> matrix2x4;

    typedef matrix<3, 1> matrix3x1;
    typedef matrix<3, 2> matrix3x2;
    typedef matrix<3, 3> matrix3x3;
    typedef matrix<3, 4> matrix3x4;

    typedef matrix<4, 1> matrix4x1;
    typedef matrix<4, 2> matrix4x2;
    typedef matrix<4, 3> matrix4x3;
    typedef matrix<4, 4> matrix4x4;
}