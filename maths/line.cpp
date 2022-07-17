#include "line.h"

namespace mkr {
    const line line::x_axis = line{vector3::zero, vector3::x_direction};
    const line line::y_axis = line{vector3::zero, vector3::y_direction};
    const line line::z_axis = line{vector3::zero, vector3::z_direction};

    line::line(const vector3& _point, const vector3& _direction)
            :point_(_point), direction_(_direction) { }

    bool line::operator==(const line& _line) const
    {
        return is_parallel(_line) && contains(_line.point_);
    }

    bool line::is_parallel(const line& _line) const
    {
        return direction_.is_parallel(_line.direction_);
    }

    bool line::is_parallel(const vector3& _vector) const
    {
        return direction_.is_parallel(_vector);
    }

    bool line::is_perpendicular(const line& _line) const
    {
        return direction_.is_perpendicular(_line.direction_);
    }

    bool line::is_perpendicular(const vector3& _vector) const
    {
        return direction_.is_perpendicular(_vector);
    }

    float line::angle_between(const line& _line) const
    {
        return direction_.angle_between(_line.direction_);
    }

    float line::angle_between(const vector3& _vector) const
    {
        return direction_.angle_between(_vector);
    }

    bool line::contains(const vector3& _point) const
    {
        return (point_==_point) || (_point-point_).is_parallel(direction_);
    }

    [[nodiscard]] vector3 line::closest_point(const vector3& _point) const
    {
        /**
         * Let the given point be P.
         * Let the closest point to P that lies on the line be C.
         * Let A be a point on the line.
         * Let D be the direction of the line.
         *
         * Since C lies on the line, it can be represented by the line equation C = A + λD --(1).
         * Since CP is perpendicular to the line, CP·D = 0.
         * This can be rewritten as (P - C)·D = 0 --(2).
         *
         * To find C, we need to find the value of λ.
         * There's 2 unknowns in our equations, C and λ, so let's get rid of one unknown.
         *
         * Subbing (1) into (2), we get the equation
         * (P - A + λD)·D = 0
         * (P - A)·D + λD·D = 0
         * (P - A)·D = -λD·D
         * (A - P)·D = λD·D
         * λ = [(A - P)·D]/(D·D)
         *
         * Therefore, C = A + {[(A - P)·D]/(D·D)}D
         */

        float lambda = direction_.dot(this->point_-_point)/direction_.length_squared();
        return point_+lambda*direction_;
    }

    std::string line::to_string() const
    {
        return "["+point_.to_string()+" + t*"+direction_.to_string()+"]";
    }

    std::ostream& operator<<(std::ostream& _os, const line& _line)
    {
        _os << _line.to_string();
        return _os;
    }
}