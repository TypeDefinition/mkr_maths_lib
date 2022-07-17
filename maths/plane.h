#pragma once

#include <optional>
#include "line.h"

namespace mkr {
    /**
     * A plane represented by the formula ax + by + cz + d = 0.
     */
    class plane {
    public:
        /// The normal of the plane.
        vector3 normal_;
        /// The constant value in the formula ax + by + cz + d = 0.
        float d_;

        bool operator==(const plane& _plane) const;

        /**
         * Creates a plane represented by the formula ax + by + cz + d = 0.
         * @param _normal The normal of the plane.
         * @param _d The constant value in the formula ax + by + cz + d = 0.
         */
        plane(const vector3& _normal, float _d);
        /**
         * Creates a plane represented by the formula ax + by + cz + d = 0.
         * @param _normal The normal of the plane.
         * @param _point_on_plane A point of the plane.
         */
        plane(const vector3& _normal, const vector3& _point_on_plane);
        /**
         * Creates a plane represented by the formula ax + by + cz + d = 0.
         * The resulting plane goes through the given three points.
         * The points go around anti-clockwise as you look down on the top surface of the plane.
         * @param _vertex_a The first point.
         * @param _vertex_b The second point.
         * @param _vertex_c The third point.
         */
        plane(const vector3& _vertex_a, const vector3& _vertex_b, const vector3& _vertex_c);

        /**
         * Returns a copy of the plane in the opposite direction.
         * @return A copy of the plane in the opposite direction.
         */
        [[nodiscard]] plane flipped() const;
        /**
         * Checks if 2 planes are parallel.
         * @param _plane The plane to check against.
         * @return Returns true if the 2 planes are parallel, else return false.
         */
        [[nodiscard]] bool is_parallel(const plane& _plane) const;
        /**
         * Checks if a line is parallel to this plane.
         * @param _line The line to check against.
         * @return Returns true if the line is parallel to this plane, else return false.
         */
        [[nodiscard]] bool is_parallel(const line& _line) const;
        /**
         * Checks if a vector is parallel to this plane.
         * @param _vector The line to check against.
         * @return Returns true if the vector is parallel to this plane, else return false.
         */
        [[nodiscard]] bool is_parallel(const vector3& _vector) const;
        /**
         * Checks if 2 planes are perpendicular.
         * @param _plane The plane to check against.
         * @return Returns true if the 2 planes are perpendicular, else return false.
         */
        [[nodiscard]] bool is_perpendicular(const plane& _plane) const;
        /**
         * Checks if a line is perpendicular to this plane.
         * @param _line The line to check against.
         * @return Returns true if the line is perpendicular to this plane, else return false.
         */
        [[nodiscard]] bool is_perpendicular(const line& _line) const;
        /**
         * Checks if a vector is perpendicular to this plane.
         * @param _vector The line to check against.
         * @return Returns true if the vector is perpendicular to this plane, else return false.
         */
        [[nodiscard]] bool is_perpendicular(const vector3& _vector) const;
        /**
         * Returns the distance of this plane from the origin.
         * @return The distance of this plane from the origin.
         */
        [[nodiscard]] float distance_from_origin() const;
        /**
         * Returns the distance of this plane to a point.
         * @return The distance of this plane to a point.
         */
        [[nodiscard]] float distance_to(const vector3& _point) const;
        /**
         * Returns the angle between 2 planes.
         * @param _plane The plane to check against.
         * @return The angle between 2 planes.
         */
        [[nodiscard]] float angle_between(const plane& _plane) const;
        /**
         * Returns the angle between this plane and a line.
         * @param _line The line to check against.
         * @return The angle between this plane and a line.
         */
        [[nodiscard]] float angle_between(const line& _line) const;
        /**
         * Returns the angle between this plane and a vector.
         * @param _vector The vector to check against.
         * @return The angle between this plane and a vector.
         */
        [[nodiscard]] float angle_between(const vector3& _vector) const;
        /**
         * Checks if a point lies on the plane.
         * @param _point The point to check.
         * @return Returns true if the point lies on the plane, else return false.
         */
        [[nodiscard]] bool contains(const vector3& _point) const;
        /**
         * Checks if a line lies on the plane.
         * @param _line The line to check.
         * @return Returns true if the line lies on the plane, else return false.
         */
        [[nodiscard]] bool contains(const line& _line) const;
        /**
         * Returns the point on this plane closest to a given point.
         * @param _point The given point.
         * @return The point on this plane closest to a given point.
         */
        [[nodiscard]] vector3 closest_point(const vector3& _point) const;
        /**
         * Return the intersection point between this plane and a given line.
         * @param _line The given line.
         * @return The intersection point between this plane and a given line.
         * @attention Returns std::nullopt if the line does not intersect this plane.
         */
        [[nodiscard]] std::optional<vector3> intersect_point(const line& _line) const;
        /**
         * Return the line of intersection between 2 planes.
         * @param _line The plane to intersect with.
         * @return The line of intersection between 2 planes.
         * @attention Returns std::nullopt if the 2 planes do not intersect.
         */
        [[nodiscard]] std::optional<line> intersect_line(const plane& _plane) const;
        /**
         * Returns a string representation of this plane.
         * @return A string representation of this plane.
         */
        [[nodiscard]] std::string to_string() const;

        friend std::ostream& operator<<(std::ostream& _os, const plane& _plane);
    };
}