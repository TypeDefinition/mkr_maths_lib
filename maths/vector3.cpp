#include "vector3.h"

namespace mkr {
    const vector3 vector3::zero = vector3{0.0f, 0.0f, 0.0f};
    const vector3 vector3::up = vector3{0.0f, 1.0f, 0.0f};
    const vector3 vector3::down = vector3{0.0f, -1.0f, 0.0f};
    const vector3 vector3::left = vector3{1.0f, 0.0f, 0.0f};
    const vector3 vector3::right = vector3{-1.0f, 0.0f, 0.0f};
    const vector3 vector3::forwards = vector3{0.0f, 0.0f, 1.0f};
    const vector3 vector3::backwards = vector3{0.0f, 0.0f, -1.0f};
    const vector3 vector3::x_direction = vector3{1.0f, 0.0f, 0.0f};
    const vector3 vector3::y_direction = vector3{0.0f, 1.0f, 0.0f};
    const vector3 vector3::z_direction = vector3{0.0f, 0.0f, 1.0f};

    vector3::vector3(float _x, float _y, float _z)
            :x_(_x), y_(_y), z_(_z) { }

    vector3 vector3::operator-() const
    {
        return vector3(-x_, -y_, -z_);
    }

    bool vector3::operator==(const vector3& _rhs) const
    {
        return maths_util::approx_equal(x_, _rhs.x_) &&
                maths_util::approx_equal(y_, _rhs.y_) &&
                maths_util::approx_equal(z_, _rhs.z_);
    }

    bool vector3::operator!=(const vector3& _rhs) const
    {
        return !(*this==_rhs);
    }

    vector3 vector3::operator+(const vector3& _rhs) const
    {
        return vector3(x_+_rhs.x_, y_+_rhs.y_, z_+_rhs.z_);
    }

    vector3& vector3::operator+=(const vector3& _rhs)
    {
        x_ += _rhs.x_;
        y_ += _rhs.y_;
        z_ += _rhs.z_;
        return *this;
    }

    vector3 vector3::operator-(const vector3& _rhs) const
    {
        return vector3(x_-_rhs.x_, y_-_rhs.y_, z_-_rhs.z_);
    }

    vector3& vector3::operator-=(const vector3& _rhs)
    {
        x_ -= _rhs.x_;
        y_ -= _rhs.y_;
        z_ -= _rhs.z_;
        return *this;
    }

    vector3 vector3::operator*(float _scalar) const
    {
        return vector3(_scalar*x_, _scalar*y_, _scalar*z_);
    }

    vector3& vector3::operator*=(float _scalar)
    {
        x_ *= _scalar;
        y_ *= _scalar;
        z_ *= _scalar;
        return *this;
    }

    void vector3::normalise()
    {
        float length = this->length();
        if (maths_util::approx_equal(length, 0.0f)) {
            set_to_zero();
            return;
        }
        x_ /= length;
        y_ /= length;
        z_ /= length;
    }

    vector3 vector3::normalised() const
    {
        float length = this->length();
        if (maths_util::approx_equal(length, 0.0f)) {
            return vector3::zero;
        }
        return vector3{x_/length, y_/length, z_/length};
    }

    void vector3::set_to_zero() { x_ = y_ = z_ = 0.0f; }

    bool vector3::is_zero_vector() const
    {
        return maths_util::approx_equal(0.0f, length_squared());
    }

    bool vector3::is_unit_vector() const
    {
        return maths_util::approx_equal(1.0f, length_squared());
    }

    bool vector3::is_parallel(const vector3& _vector) const
    {
        return !is_zero_vector() &&
                !_vector.is_zero_vector() &&
                maths_util::approx_equal(0.0f, cross(_vector).length_squared());
    }

    bool vector3::is_perpendicular(const vector3& _vector) const
    {
        return !is_zero_vector() &&
                !_vector.is_zero_vector() &&
                maths_util::approx_equal(0.0f, dot(_vector));
    }

    float vector3::length() const
    {
        return std::sqrt(length_squared());
    }

    float vector3::length_squared() const
    {
        return x_*x_+y_*y_+z_*z_;
    }

    float vector3::dot(const vector3& _vector) const
    {
        return x_*_vector.x_+y_*_vector.y_+z_*_vector.z_;
    }

    vector3 vector3::project(const vector3& _vector) const
    {
        float other_length_squared = _vector.length_squared();
        if (maths_util::approx_equal(other_length_squared, 0.0f)) {
            return vector3::zero;
        }
        return (dot(_vector)*_vector)*(1.0f/other_length_squared);
    }

    float vector3::angle_between(const vector3& _vector) const
    {
        return std::acos(dot(_vector)/(length()*_vector.length()));
    }

    vector3 vector3::cross(const vector3& _vector) const
    {
        float x = y_*_vector.z_-z_*_vector.y_;
        float y = z_*_vector.x_-x_*_vector.z_;
        float z = x_*_vector.y_-y_*_vector.x_;
        return vector3(x, y, z);
    }

    std::string vector3::to_string() const
    {
        return "("+std::to_string(x_)+", "+std::to_string(y_)+", "+std::to_string(z_)+")";
    }

    vector3 operator*(float _scalar, const vector3& _vector)
    {
        return _vector*_scalar;
    }

    std::ostream& operator<<(std::ostream& _os, const vector3& _vector)
    {
        _os << _vector.to_string();
        return _os;
    }
}