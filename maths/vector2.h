#pragma once

#include <sstream>
#include "maths_util.h"

namespace mkr {
    /**
     * A 2D vector with x and y components.
     */
    class vector2 {
    public:
        /// The x component.
        float x_;
        /// The y component.
        float y_;

        static const vector2 zero;
        static const vector2 up;
        static const vector2 down;
        static const vector2 left;
        static const vector2 right;
        static const vector2 x_direction;
        static const vector2 y_direction;

        /**
         * Constructs the vector.
         * @param _x The x component.
         * @param _y The y component.
         */
        vector2(float _x = 0.0f, float _y = 0.0f);

        vector2 operator-() const;
        bool operator==(const vector2& _rhs) const;
        bool operator!=(const vector2& _rhs) const;
        vector2 operator+(const vector2& _rhs) const;
        vector2& operator+=(const vector2& _rhs);
        vector2 operator-(const vector2& _rhs) const;
        vector2& operator-=(const vector2& _rhs);
        vector2 operator*(float _scalar) const;
        vector2& operator*=(float _scalar);

        /**
         * Normalise this vector.
         */
        void normalise();
        /**
         * Returns a normalised copy of this vector.
         * @return A normalised copy of this vector.
         */
        [[nodiscard]] vector2 normalised() const;
        /**
         * Sets this vector to (0, 0).
         */
        void set_to_zero();
        /**
         * Checks if this vector is a zero vector.
         * @return Returns true if the vector is a zero vector, else return false.
         */
        [[nodiscard]] bool is_zero_vector() const;
        /**
         * Checks if this vector is a unit vector.
         */
        [[nodiscard]] bool is_unit_vector() const;
        /**
         * Checks if 2 vectors are parallel.
         * @param _vector The vector to compare to.
         * @return Returns true if the 2 vectors are parallel, else returns false.
         */
        [[nodiscard]] bool is_parallel(const vector2& _vector) const;
        /**
         * Checks if 2 vectors are perpendicular.
         * @param _vector The vector to compare to.
         * @return Returns true if the 2 vectors are perpendicular, else returns false.
         */
        [[nodiscard]] bool is_perpendicular(const vector2& _vector) const;
        /**
         * Returns the length of this vector.
         * @return The length of this vector.
         */
        [[nodiscard]] float length() const;
        /**
         * Returns the squared length of this vector.
         * @return The squared length of this vector.
         */
        [[nodiscard]] float length_squared() const;
        /**
         * Returns the dot product of 2 vectors.
         * @param _vector　The vector to dot with.
         * @return The dot product of 2 vectors.
         */
        [[nodiscard]] float dot(const vector2& _vector) const;
        /**
         * Returns the projection of this vector onto another vector.
         * @param _vector The vector to project this vector onto.
         * @return The projection of this vector onto another vector.
         */
        [[nodiscard]] vector2 project(const vector2& _vector) const;
        /**
         * Returns the angle between 2 vectors.
         * @param _vector The other vector to find the angle with.
         * @return The angle between 2 vectors.
         */
        [[nodiscard]] float angle_between(const vector2& _vector) const;
        /**
         * Returns a string representation of this vector.
         * @return A string representation of this vector.
         */
        [[nodiscard]] std::string to_string() const;

        friend vector2 operator*(float _scalar, const vector2& _vector);
        friend std::ostream& operator<<(std::ostream& _os, const vector2& _vector);
    };
}