#pragma once

#include <sstream>
#include "maths_util.h"

namespace mkr {
    /**
     * Represents a colour with the components RGBA.
     * Each component is usually within the range of 0 to 1.
     */
    class colour {
    public:
        /// Red component.
        float r_;
        /// Green component.
        float g_;
        /// Blue component.
        float b_;
        /// Alpha component.
        float a_;

        static const colour red;
        static const colour green;
        static const colour blue;
        static const colour yellow;
        static const colour cyan;
        static const colour magenta;
        static const colour black;
        static const colour white;
        static const colour grey;

        /**
         * Constructs the colour.
         * @param _r The red component of the colour.
         * @param _g The green component of the colour.
         * @param _b The blue component of the colour.
         * @param _a The alpha component of the colour.
         */
        colour(float _r = 1.0f, float _g = 1.0f, float _b = 1.0f, float _a = 1.0f);

        bool operator==(const colour& _rhs) const;
        bool operator!=(const colour& _rhs) const;
        colour operator+(const colour& _rhs) const;
        colour& operator+=(const colour& _rhs);
        colour operator-(const colour& _rhs) const;
        colour& operator-=(const colour& _rhs);
        colour operator*(const colour& _rhs) const;
        colour& operator*=(const colour& _rhs);
        colour operator*(float _scalar) const;
        colour& operator*=(float _scalar);

        /**
         * Returns a string representation of this colour.
         * @return A string representation of this colour.
         */
        [[nodiscard]] std::string to_string() const;

        friend colour operator*(float _scalar, const colour& _colour);
        friend std::ostream& operator<<(std::ostream& _os, const colour& _colour);
    };
}