#pragma once

#include <limits>
#include <type_traits>
#include <cmath>

namespace mkr {
    /**
     * A maths utility class.
     */
    class maths_util {
    public:
        maths_util() = delete;

        static constexpr float pi = 3.14159265358979323846f;
        static constexpr float rad2deg = 180.0f / pi;
        static constexpr float deg2rad = pi / 180.0f;

        /**
         * Return the smaller of the given values.
         * @tparam T Value type.
         * @param _a First value to compare.
         * @param _b Second value to compare.
         * @return The smaller of _a and _b. If the values are equivalent, returns _a.
         */
        template<class T>
        static const T &min(const T &_a, const T &_b) {
            return (_b < _a) ? _b : _a;
        }

        /**
         * Return the smaller of the given values.
         * @tparam T Value type.
         * @tparam Args Variadic template type.
         * @param _a First value to compare.
         * @param _b Second value to compare.
         * @param _args The third to nth number to compare.
         * @return The smaller of _a and _b. If the values are equivalent, returns _a.
         */
        template<class T, class ...Args>
        static const T &min(const T &_a, const T &_b, Args &&... _args) {
            return maths_util::min<T>(_a, maths_util::min<T>(_b, std::forward<Args>(_args)...));
        }

        /**
         * Return the larger of the given values.
         * @tparam T Value type.
         * @param _a First value to compare.
         * @param _b Second value to compare.
         * @return The larger of _a and _b. If the values are equivalent, returns _a.
         */
        template<class T>
        static const T &max(const T &_a, const T &_b) {
            return (_a < _b) ? _b : _a;
        }

        /**
         * Return the larger of the given values.
         * @tparam T Value type.
         * @tparam Args Variadic template type.
         * @param _a First value to compare.
         * @param _b Second value to compare.
         * @param _args The third to nth number to compare.
         * @return The larger of _a and _b. If the values are equivalent, returns _a.
         */
        template<class T, class ...Args>
        static const T &max(const T &_a, const T &_b, Args &&... _args) {
            return maths_util::max<T>(_a, maths_util::max<T>(_b, std::forward<Args>(_args)...));
        }

        /**
         * Checks if 2 floating point numbers are approximately equal. Useful for dealing with floating point errors.
         * @tparam T The floating point type.
         * @param _a The first number to compare.
         * @param _b The second number to compare.
         * @return Returns true if both numbers are approximately equal. Else, returns false.
         */
        template<class T>
        static std::enable_if_t<!std::numeric_limits<T>::is_integer, bool> approx_equal(const T &_a, const T &_b) {
            return std::fabs(_a - _b) <= std::numeric_limits<T>::epsilon();
        }

        /**
         * Checks if N floating point numbers are approximately equal. Useful for dealing with floating point errors.
         * @tparam T The floating point type.
         * @tparam Args The floating point type.
         * @param _a The first number to compare.
         * @param _b The second number to compare.
         * @param _args The third to nth number to compare.
         * @return Returns true if all numbers are approximately equal. Else, returns false.
         */
        template<class T, class... Args>
        static std::enable_if_t<!std::numeric_limits<T>::is_integer, bool>
        approx_equal(const T &_a, const T &_b, Args &&... _args) {
            return std::fabs(maths_util::max<T>(_a, _b, std::forward<Args>(_args)...) -
                             maths_util::min<T>(_a, _b, std::forward<Args>(_args)...)) <=
                   std::numeric_limits<T>::epsilon();
        }
    };
}