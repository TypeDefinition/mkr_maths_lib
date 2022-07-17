#include <stdexcept>
#include "quaternion.h"

namespace mkr {
    const quaternion quaternion::identity = quaternion{1.0f, 0.0f, 0.0f, 0.0f};
    const quaternion quaternion::zero = quaternion{0.0f, 0.0f, 0.0f, 0.0f};

    vector3 quaternion::rotate(const vector3& _point, float _angle, const vector3& _rotation_axis)
    {
        quaternion rotation{_angle, _rotation_axis};
        quaternion point{0.0f, _point.x_, _point.y_, _point.z_};
        quaternion result = rotation*point*rotation.inversed();

        return vector3{result.x_, result.y_, result.z_};
    }

    quaternion quaternion::slerp(const quaternion& _start, const quaternion& _end, float _ratio, bool _clamp_ratio)
    {
        if (!_start.is_unit_quaternion() || !_end.is_unit_quaternion()) {
            throw std::invalid_argument("quaternion::slerp only accepts unit quaternions as arguments!");
        }

        _ratio = _clamp_ratio ? std::clamp(_ratio, 0.0f, 1.0f) : _ratio;

        /*
        Let start quaternion be s.
        Let end quaternion be e.
        Let the quaternion needed to transform s to e be d (difference).

        We need to find what is d.
        d * s = e
        Therefore,
        d * s * s.Inversed = e * s.Inversed [s * s.Inversed cancels each other out.]
        d = e * s.inversed
        */
        quaternion d = _end*_start.inversed();

        /*
        Since s * d = e, if we want for example to only turn halfway, then we need to half the angle that d turns.
        For example, d could represent a rotation of 90 degrees around the Vector::UP axis.
        So to do a half rotation, we need to do a rotation of 45 degress around the Vector::UP axis.
        To do that, we must first figure out what the angle and axis of d is.
        */
        float d_angle;
        vector3 d_axis;
        d.to_axis_angle(d_angle, d_axis);

        // Now that we have the angle and axis to rotate around, we have our result.
        return quaternion(d_angle*_ratio, d_axis)*_start;
    }

    quaternion::quaternion(float _w, float _x, float _y, float _z)
            :w_(_w), x_(_x), y_(_y), z_(_z) { }

    quaternion::quaternion(float _angle, const vector3& _rotation_axis)
    {
        set_to_rotation(_angle, _rotation_axis);
    }

    bool quaternion::operator==(const quaternion& _rhs) const
    {
        return maths_util::approx_equal(w_, _rhs.w_) &&
                maths_util::approx_equal(x_, _rhs.x_) &&
                maths_util::approx_equal(y_, _rhs.y_) &&
                maths_util::approx_equal(z_, _rhs.z_);
    }

    bool quaternion::operator!=(const quaternion& _rhs) const
    {
        return !((*this)==_rhs);
    }

    quaternion quaternion::operator*(const quaternion& _rhs) const
    {
        vector3 this_vector{x_, y_, z_};
        vector3 rhs_vector{_rhs.x_, _rhs.y_, _rhs.z_};

        float w = w_*_rhs.w_-this_vector.dot(rhs_vector);
        vector3 xyz = w_*rhs_vector+_rhs.w_*this_vector+this_vector.cross(rhs_vector);

        return quaternion(w, xyz.x_, xyz.y_, xyz.z_);
    }

    quaternion& quaternion::operator*=(const quaternion& _rhs)
    {
        *this = (*this)*_rhs;
        return *this;
    }

    void quaternion::normalise()
    {
        float length = this->length();
        if (maths_util::approx_equal(length, 0.0f)) {
            set_to_zero();
            return;
        }
        w_ /= length;
        x_ /= length;
        y_ /= length;
        z_ /= length;
    }

    quaternion quaternion::normalised() const
    {
        float length = this->length();
        if (maths_util::approx_equal(length, 0.0f)) {
            return quaternion::zero;
        }
        return quaternion(w_/length, x_/length, y_/length, z_/length);
    }

    void quaternion::set_to_zero() { w_ = x_ = y_ = z_ = 0.0f; }

    bool quaternion::is_zero_quaternion() const
    {
        return maths_util::approx_equal(0.0f, length_squared());
    }

    bool quaternion::is_unit_quaternion() const
    {
        return maths_util::approx_equal(1.0f, length_squared());
    }

    float quaternion::length() const
    {
        return std::sqrt(length_squared());
    }

    float quaternion::length_squared() const
    {
        return (w_*w_)+(x_*x_)+(y_*y_)+(z_*z_);
    }

    void quaternion::inverse()
    {
        x_ = -x_;
        y_ = -y_;
        z_ = -z_;
    }

    quaternion quaternion::inversed() const
    {
        return quaternion(w_, -x_, -y_, -z_);
    }

    float quaternion::dot(const quaternion& _quaternion) const
    {
        return (w_*_quaternion.w_)+(x_*_quaternion.x_)+(y_*_quaternion.y_)+(z_*_quaternion.z_);
    }

    bool quaternion::to_axis_angle(float& _angle, vector3& _rotation_axis) const
    {
        if (!is_unit_quaternion()) {
            return false;
        }
        _angle = std::acos(w_)*2.0f;
        // We simply normalize xyz since the direction of xyz is the same as the rotation axis.
        _rotation_axis = vector3{x_, y_, z_}.normalised();
        return true;
    }

    matrix4x4 quaternion::to_rotation_matrix() const
    {
        matrix4x4 matrix_a;

        matrix_a[0][0] = w_;
        matrix_a[1][1] = w_;
        matrix_a[2][2] = w_;
        matrix_a[3][3] = w_;

        matrix_a[0][3] = x_;
        matrix_a[1][2] = x_;
        matrix_a[2][1] = -x_;
        matrix_a[3][0] = -x_;

        matrix_a[0][2] = -y_;
        matrix_a[1][3] = y_;
        matrix_a[2][0] = y_;
        matrix_a[3][1] = -y_;

        matrix_a[0][1] = z_;
        matrix_a[1][0] = -z_;
        matrix_a[2][3] = z_;
        matrix_a[3][2] = -z_;

        matrix4x4 matrix_b;

        matrix_b[0][0] = w_;
        matrix_b[1][1] = w_;
        matrix_b[2][2] = w_;
        matrix_b[3][3] = w_;

        matrix_b[0][3] = -x_;
        matrix_b[1][2] = x_;
        matrix_b[2][1] = -x_;
        matrix_b[3][0] = x_;

        matrix_b[0][2] = -y_;
        matrix_b[1][3] = -y_;
        matrix_b[2][0] = y_;
        matrix_b[3][1] = y_;

        matrix_b[0][1] = z_;
        matrix_b[1][0] = -z_;
        matrix_b[2][3] = -z_;
        matrix_b[3][2] = z_;

        return matrix_a*matrix_b;
    }

    void quaternion::set_to_rotation(float _angle, const vector3& _rotation_axis)
    {
        // If the rotation axis is invalid, set this to a identity quaternion.
        if (_rotation_axis.is_zero_vector()) {
            *this = quaternion::identity;
        }
        else {
            vector3 xyz = std::sin(_angle*0.5f)*_rotation_axis.normalised();
            x_ = xyz.x_;
            y_ = xyz.y_;
            z_ = xyz.z_;
            w_ = std::cos(_angle*0.5f);
        }
    }
}