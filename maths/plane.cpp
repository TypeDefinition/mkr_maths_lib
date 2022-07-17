#include "plane.h"

namespace mkr {
    plane::plane(const vector3& _normal, float _d)
            :normal_(_normal), d_(_d) { }

    plane::plane(const vector3& _normal, const vector3& _point_on_plane)
            :normal_(_normal), d_(-_normal.dot(_point_on_plane)) { }

    plane::plane(const vector3& _vertex_a, const vector3& _vertex_b, const vector3& _vertex_c)
    {
        vector3 edge_ab = _vertex_b-_vertex_a;
        vector3 edge_ac = _vertex_c-_vertex_a;
        normal_ = edge_ab.cross(edge_ac);
        d_ = -normal_.dot(_vertex_a);
    }

    bool plane::operator==(const plane& _plane) const
    {
        /// For 2 planes to be equal, they must face the same direction and be the same distance from the origin.
        return is_parallel(_plane) &&
                maths_util::approx_equal(distance_from_origin(), _plane.distance_from_origin());
    }

    plane plane::flipped() const
    {
        return plane(-normal_, d_);
    }

    bool plane::is_parallel(const plane& _plane) const
    {
        return normal_.is_parallel(_plane.normal_);
    }

    bool plane::is_parallel(const line& _line) const
    {
        return normal_.is_perpendicular(_line.direction_);
    }

    bool plane::is_parallel(const vector3& _vector) const
    {
        return normal_.is_perpendicular(_vector);
    }

    bool plane::is_perpendicular(const plane& _plane) const
    {
        return normal_.is_perpendicular(_plane.normal_);
    }

    bool plane::is_perpendicular(const line& _line) const
    {
        return normal_.is_parallel(_line.direction_);
    }

    bool plane::is_perpendicular(const vector3& _vector) const
    {
        return normal_.is_parallel(_vector);
    }

    float plane::distance_from_origin() const
    {
        return d_/normal_.length();
    }

    float plane::distance_to(const vector3& _point) const
    {
        /**
         * We are given a point P.
         * Let's call the closest point from point P to the plane point C.
         * Now imagine an arbitrary point on the plane, point B.
         * Let the angle between vector BP and vector CP be θ.
         *
         * Point C |--------------> P
         *         |          θ /
         *         |          /
         *         |        /
         *         |      /
         *         |    /
         *         |  /
         *         |/ θ
         * Point B |------------------------> Normal
         *         |
         *
         * What we want to do is to find the length of CP.
         *
         * cosθ = |CP|/|BP|
         * |CP| = |BP|cosθ
         *
         * The problem is, we do not know what the value of |BP|cosθ is.
         * What we do know, is that if we take the product of BP and the Normal, we have the following:
         * BP·Normal = |BP||Normal|cosθ
         * |BP|cosθ = (BP·Normal)/|Normal|
         * |CP| = (BP·Normal)/|Normal|
         *
         * Okay, all is well, but we don't know what B is. But let's expand the equation first and figure it out along.
         *
         * |CP| = (BP·Normal)/|Normal|
         * |CP| = (P·Normal - B·Normal)/|Normal|
         *
         * Okay okay, see something there? Look at B·Normal, where B is just any point on our plane. Now where have I seen that before?
         * Oh right, our plane formula, which is A·Normal - d = 0, where A is just any point on our plane.
         *
         * So we can sub B into A.
         * B·Normal - d = 0
         * B·Normal = -d
         *
         * Thus we can conclude that,
         * |CP| = (P·Normal - B·Normal)/|Normal|
         * |CP| = (P·Normal + d)/|Normal|
         */

        return (normal_.dot(_point)+d_)/normal_.length();
    }

    float plane::angle_between(const plane& _plane) const
    {
        return normal_.angle_between(_plane.normal_);
    }

    float plane::angle_between(const vector3& _vector) const
    {
        return std::asin(normal_.dot(_vector)/(normal_.length()*_vector.length()));
    }

    float plane::angle_between(const line& _line) const
    {
        return std::asin(normal_.dot(_line.direction_)/(normal_.length()*_line.direction_.length()));
    }

    bool plane::contains(const vector3& _point) const
    {
        return maths_util::approx_equal(-d_, normal_.dot(_point));
    }

    bool plane::contains(const line& _line) const
    {
        return is_parallel(_line) && contains(_line.point_);
    }

    vector3 plane::closest_point(const vector3& _point) const
    {
        /**
         * Let any point on the plane be represented by A.
         * Plane equation: A·N + d = 0.
         *
         * Let the given point be P.
         * Let the closest point to P that lies on the plane be C.
         * Let the normal of the plane be N.
         *
         * The equation of line CP can be written as P + λN.
         * Thus, C can be expressed as C = P + λN. --Equation (1)
         *
         * Since C lies on the plane, the equation C·N + d = 0 must be true. --Equation (2)
         *
         * What we need in order to solve for C, is to find the value of λ.
         *
         * Subbing (1) into (2),
         * (P + λN)·N + d = 0
         * (P + λN)·N = -d
         * P·N + λ(N·N) = -d
         * λ(N·N) = -d - (P·N)
         * λ = [-d - (P·N)] / (N·N)
         * λ = [-d - (P·N)] / (|N|*|N|)
         *
         * Thus, C = P + {[-d - (P·N)] / (|N|*|N|)} * N
         */

        float lambda = (-d_-normal_.dot(_point))/normal_.length_squared();
        return _point+lambda*normal_;
    }

    std::optional<vector3> plane::intersect_point(const line& _line) const
    {
        /**
         * A plane and a line intersects at a point assuming they are not parallel.
         *
         * Let any point on the plane be represented by A.
         * Plane equation: A·N + d = 0.
         *
         * Let the intersection point be C.
         * Let the line origin be P.
         * Let the line direction be D.
         * Let the normal of the plane be N.
         *
         * The equation of line CP can be written as P + λD.
         * Thus, C can be expressed as C = P + λD. --Equation (1)
         *
         * Since C lies on the plane, the equation C·N + d = 0 must be true. --Equation (2)
         *
         * What we need in order to solve for C, is to find the value of λ.
         *
         * Subbing (1) into (2),
         * (P + λD)·N + d = 0
         * (P + λD)·N = -d
         * P·N + λ(D·N) = -d
         * λ(D·N) = -d - (P·N)
         * λ = [-d - (P·N)] / (D·N)
         *
         * Thus, C = P + {[-d - (P·N)] / (D·N)} * D
         */
        // The line and plane do not intersect if they are parallel.
        if (is_parallel(_line)) {
            return std::nullopt;
        }

        float lambda = (-d_-normal_.dot(_line.point_))/normal_.dot(_line.direction_);
        return _line.point_+_line.direction_*lambda;
    }

    std::optional<line> plane::intersect_line(const plane& _plane) const
    {
        /**
         * 2 planes intersect at a line if they are not parallel.
         * [https://stackoverflow.com/questions/6408670/line-of-intersection-between-two-planes/17628505]
         *
         * Let plane a be this plane.
         * Let plane b be _plane.
         * Let plane c be a plane whose normal is the cross product of plane 1 & plane 2, and passes through the origin.
         */
        vector3 plane_c_normal = normal_.cross(_plane.normal_);
        float determinant = plane_c_normal.length_squared();

        /// There is no point of intersection of the planes are parallel.
        if (maths_util::approx_equal(0.0f, determinant)) {
            return std::nullopt;
        }

        vector3 b_cross_c = _plane.normal_.cross(plane_c_normal);
        vector3 c_cross_a = plane_c_normal.cross(normal_);
        vector3 point_on_line = ((-d_*b_cross_c)+(-_plane.d_*c_cross_a))*(1.0f/determinant);

        return line{point_on_line, plane_c_normal};
    }

    std::string plane::to_string() const
    {
        return "(x*"+std::to_string(normal_.x_)+" + y*"+std::to_string(normal_.y_)+" + z*"+std::to_string(normal_.z_)
                +" + "+std::to_string(d_)+")";
    }

    std::ostream& operator<<(std::ostream& _os, const plane& _plane)
    {
        _os << _plane.to_string();
        return _os;
    }
}