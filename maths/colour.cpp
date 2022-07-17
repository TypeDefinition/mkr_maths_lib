#include "colour.h"

namespace mkr {
    const colour colour::red{1.0f, 0.0f, 0.0f, 1.0f};
    const colour colour::green{0.0f, 1.0f, 0.0f, 1.0f};
    const colour colour::blue{0.0f, 0.0f, 1.0f, 1.0f};
    const colour colour::yellow{1.0f, 1.0f, 0.0f, 1.0f};
    const colour colour::cyan{0.0f, 1.0f, 1.0f, 1.0f};
    const colour colour::magenta{1.0f, 0.0f, 1.0f, 1.0f};
    const colour colour::black{0.0f, 0.0f, 0.0f, 1.0f};
    const colour colour::white{1.0f, 1.0f, 1.0f, 1.0f};
    const colour colour::grey{0.25f, 0.25f, 0.25f, 1.0f};

    colour::colour(float _r, float _g, float _b, float _a)
            :r_(_r), g_(_g), b_(_b), a_(_a) { }

    bool colour::operator==(const colour& _rhs) const
    {
        return maths_util::approx_equal(r_, _rhs.r_) &&
                maths_util::approx_equal(g_, _rhs.g_) &&
                maths_util::approx_equal(b_, _rhs.b_) &&
                maths_util::approx_equal(a_, _rhs.a_);
    }

    bool colour::operator!=(const colour& _rhs) const
    {
        return !(*this==_rhs);
    }

    colour colour::operator+(const colour& _rhs) const
    {
        return colour(r_+_rhs.r_, g_+_rhs.g_, b_+_rhs.b_, a_+_rhs.a_);
    }

    colour& colour::operator+=(const colour& _rhs)
    {
        r_ += _rhs.r_;
        g_ += _rhs.g_;
        b_ += _rhs.b_;
        a_ += _rhs.a_;
        return *this;
    }

    colour colour::operator-(const colour& _rhs) const
    {
        return colour(r_-_rhs.r_, g_-_rhs.g_, b_-_rhs.b_, a_-_rhs.a_);
    }

    colour& colour::operator-=(const colour& _rhs)
    {
        r_ -= _rhs.r_;
        g_ -= _rhs.g_;
        b_ -= _rhs.b_;
        a_ -= _rhs.a_;
        return *this;
    }

    colour colour::operator*(const colour& _rhs) const
    {
        return colour(r_*_rhs.r_, g_*_rhs.g_, b_*_rhs.b_, a_*_rhs.a_);
    }

    colour& colour::operator*=(const colour& _rhs)
    {
        r_ *= _rhs.r_;
        g_ *= _rhs.g_;
        b_ *= _rhs.b_;
        a_ *= _rhs.a_;
        return *this;
    }

    colour colour::operator*(float _scalar) const
    {
        return colour(r_*_scalar, g_*_scalar, b_*_scalar, a_*_scalar);
    }

    colour& colour::operator*=(float _scalar)
    {
        r_ *= _scalar;
        g_ *= _scalar;
        b_ *= _scalar;
        a_ *= _scalar;
        return *this;
    }

    std::string colour::to_string() const
    {
        return "("+std::to_string(r_)+", "+std::to_string(g_)+", "+std::to_string(b_)+", "+std::to_string(a_)+")";
    }

    colour operator*(float _scalar, const colour& _colour)
    {
        return _colour*_scalar;
    }

    std::ostream& operator<<(std::ostream& _os, const colour& _colour)
    {
        _os << _colour.to_string();
        return _os;
    }
}