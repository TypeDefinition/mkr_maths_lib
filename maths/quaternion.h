#pragma once

#include "vector3.h"
#include "matrix.h"

namespace mkr {
    /**
     * @brief
     * Quaternion
     *
     *    After reading a metric fuck ton of materials, here is HOW to use Quaternions.
     *    I do not understand and cannot explain WHY these things work they way the do.
     *    I can only memorize the formulas and plug them in when necessary.
     *
     *    Disclaimer:\n
     *    Anything I write below is what I THINK I understand about Quaternions.
     *    That means that it could all be wrong and I'm full of shit.
     *    But hey, I tried.
     *
     *    To Future Me:\n
     *    Hopefully you've (I've? we've?) figured it out eventually.
     *
     *    If for some reason someone other than me is reading:\n
     *    If you actually know this shit please teach me.
     *
     *    Quaternions may seem like a Vector, but with 4 elements.
     *    However, when I try to make sense of it, it simply does not make sense.
     *    Such as why when using a rotation Quaternion we need to multiply W by cos(θ)
     *    and XYZ by sin(θ).
     *
     *    How I understand Quaternions is that they are like Matrices.
     *    Unlike a Vectors, which are an actual thing, such
     *    as a direction or position, Matrices aren't. Matrices are like a tool
     *    that we plug numbers into in order to do multiplication and shit and
     *    get a result. So like the matrix that REPRESENTS a position may be |x|
     *                                                                       |y|
     *                                                                       |z|
     *                                                                       |1|,
     *    but it isn't a position. It is simply a matrix with the numbers of a position plugged in.
     *    So quaternions are kinda similar-ish. It looks like it is a rotation, but it is
     *    more of a tool that plug numbers into to REPRESENT a rotation.
     *
     *    Multiplication of Basis Elements:
     *    i^2 = j^2 = k^2 = ijk = -1
     *    ij = k
     *    ji = -k
     *    jk = i
     *    kj = -i
     *    ki = j
     *    ik = -j
     *
     *    All rotation Quaternions are Unit Quaternions.
     *    Thus, w_ = cos(angle/2) and xyz_ = sin(angle/2) * rotationAxis only applies to Unit Quaterions.
     *
     *    Maintaining Rotation During Quaternion Normalization:
     *
     *    Math answer:\n
     *    A unit quaternion represents a rotation in 3D space.
     *    Any other (i.e.: non-unit) quaternion does not represent a rotation,
     *    so the formula angle = 2 * acos(q0) does not apply to these quaternions.
     *    So there is no change of angle when normalizing,
     *    because the quaternions that you would normalize do not represent rotations in the first place.
     *
     *    Programming answer:\n
     *    Floating point operations have accuracy issues.
     *    These issues result in small errors, which if accumulated may become large errors.
     *    When multiplying two unit quaternions, the mathematical result is another unit quaternion.
     *    However the floating point implementation of unit quaternions multiplication may result in
     *    a quaternion with a norm close to 1 but not equal 1. In this case we shall normalize the quaternion
     *    to correct the error. When we normalize we divide q0 by the norm which is very close to 1, so there
     *    is no major change in the value of q0. Because we normalize early the norm is always very close
     *    to 1 and we don't need to worry about the precision.
     *
     *    Converting Quaternion To Matrix:\n
     *    When Quaternion = (w, x, y, z),\n
     *    <pre>
     *    Matrix =  | w  -x  -y  -z |\n
     *              | x   w  -z   y |\n
     *              | y   z   w  -x |\n
     *              | z  -y   x   w |\n
     *    </pre>
     */
    class quaternion {
    public:
        /**
         * The W component of the quaternion. It is the scalar component.
         * @warning Don't modify this directly unless you know quaternions inside out.
         */
        float w_;
        /**
         * The X component of the quaternion. It is part of the vector component.
         * @warning Don't modify this directly unless you know quaternions inside out.
         */
        float x_;
        /**
         * The Y component of the quaternion. It is part of the vector component.
         * @warning Don't modify this directly unless you know quaternions inside out.
         */
        float y_;
        /**
         * The Z component of the quaternion. It is part of the vector component.
         * @warning Don't modify this directly unless you know quaternions inside out.
         */
        float z_;

        static const quaternion identity;
        static const quaternion zero;

        /**
         * Rotate a point around an axis.
         * @param _point The point to rotate.
         * @param _angle The rotation angle.
         * @param _rotation_axis The rotation axis.
         * @return The rotated point.
         */
        static vector3 rotate(const vector3& _point, float _angle, const vector3& _rotation_axis);

        /**
         * Spherical Linear Interpolation between 2 rotational quaternions.
         * @param _start The start rotation.
         * @param _end The end rotation.
         * @param _ratio The ratio to interpolate between _start and _end. A value of 0 will return _start, and a value of 1 will return _end.
         * @param _clamp_ratio If set to true, the _ratio is clamped to between 0 and 1. The default value is false.
         * @return The interpolated rotation.
         * @warning _start and _end must be rotational (unit) quaternions.
         * @throw std::invalid_argument if _start or _end is not a rotational (unit) quaternions.
         */
        static quaternion
        slerp(const quaternion& _start, const quaternion& _end, float _ratio, bool _clamp_ratio = false);

        /**
         * Constructs a quaternion.
         * @param _w The W component of the quaternion. It is the scalar component.
         * @param _x The X component of the quaternion. It is part of the vector component.
         * @param _y The Y component of the quaternion. It is part of the vector component.
         * @param _z The Z component of the quaternion. It is part of the vector component.
         */
        quaternion(float _w = 1.0f, float _x = 0.0f, float _y = 0.0f, float _z = 0.0f);
        /**
         * Constructs a rotational quaternion.
         * @param _angle The rotation angle.
         * @param _rotation_axis The rotation axis.
         */
        quaternion(float _angle, const vector3& _rotation_axis);

        bool operator==(const quaternion& _rhs) const;
        bool operator!=(const quaternion& _rhs) const;
        quaternion operator*(const quaternion& _rhs) const;
        quaternion& operator*=(const quaternion& _rhs);

        /**
         * Normalise this quaternion.
         */
        void normalise();
        /**
         * Get a normalised copy of this quaternion.
         * @return A normalised copy of this quaternion.
         */
        [[nodiscard]] quaternion normalised() const;
        /**
         * Make this a zero quaternion.
         */
        void set_to_zero();
        /**
         * Checks if this quaternion is a zero quaternion.
         * @return
         */
        [[nodiscard]] bool is_zero_quaternion() const;
        /**
         * Checks if this quaternion is a unit quaternion.
         * @return
         */
        [[nodiscard]] bool is_unit_quaternion() const;

        /**
         * Return the length of the quaternion.
         * @return The length of the quaternion.
         */
        [[nodiscard]] float length() const;
        /**
         * Return the squared length of the quaternion.
         * @return The squared length of the quaternion.
         */
        [[nodiscard]] float length_squared() const;
        /**
         * Inverse this quaternion.
         */
        void inverse();
        /**
         * Get a inversed copy of this quaternion.
         * @return A inversed copy of this quaternion.
         */
        [[nodiscard]] quaternion inversed() const;
        /**
         * Get the dot product of this quaternion and another quaternion.
         * @param _quaternion The other quaternion to calculate the dot product with.
         * @return The dot product of this quaternion and the other quaternion.
         */
        [[nodiscard]] float dot(const quaternion& _quaternion) const;

        /**
         * Get this quaternion as a rotation in radians, around an axis.
         * @param _angle The angle of rotation result.
         * @param _rotation_axis The axis of rotation result.
         * @return Returns true if this quaternion is a rotation quaternion, otherwise returns false.
         * @warning This quaternion must be a rotational (unit) quaternion. If this quaternion is not a rotational (unit) quaternion, _angle and _rotation_axis' values are not set.
         */
        bool to_axis_angle(float& _angle, vector3& _rotation_axis) const;
        /**
         * Get this quaternion as a matrix4x4 rotation matrix.
         * @return This quaternion as a matrix4x4 rotation matrix.
         * @warning This function assumes that this quaternion is a rotational (unit) quaternion.
         */
        [[nodiscard]] matrix4x4 to_rotation_matrix() const;
        /**
         * Set this quaternion to a rotation given an angle in radians and an axis.
         * @param _angle The angle rotation in radians.
         * @param _rotation_axis The axis of rotation.
         */
        void set_to_rotation(float _angle, const vector3& _rotation_axis);
    };
}