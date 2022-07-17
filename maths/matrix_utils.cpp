#include "matrix_utils.h"

namespace mkr {
    matrix4x4 matrix_util::translation_matrix(const vector3& _translation)
    {
        matrix4x4 mat = matrix4x4::identity();

        mat[3][0] = _translation.x_;
        mat[3][1] = _translation.y_;
        mat[3][2] = _translation.z_;

        return mat;
    }

    matrix4x4 matrix_util::rotation_matrix_x(float _angle)
    {
        /**
         * Rotation Matrix on X-axis
         * |   1   0   0   0   |
         * |   0  cos -sin 0   |
         * |   0  sin  cos 0   |
         * |   0   0   0   1   |
         */
        matrix4x4 mat = matrix4x4::identity();
        mat[1][1] = mat[2][2] = std::cos(_angle);
        mat[1][2] = std::sin(_angle);
        mat[2][1] = -mat[1][2];

        return mat;
    }

    matrix4x4 matrix_util::rotation_matrix_y(float _angle)
    {
        /**
         * Rotation Matrix on Y-axis
         * |  cos  0  sin  0   |
         * |   0   1   0   0   |
         * | -sin  0  cos  0   |
         * |   0   0   0   1   |
         */
        matrix4x4 mat = matrix4x4::identity();
        mat[0][0] = mat[2][2] = std::cos(_angle);
        mat[2][0] = std::sin(_angle);
        mat[0][2] = -mat[2][0];

        return mat;
    }

    matrix4x4 matrix_util::rotation_matrix_z(float _angle)
    {
        /**
         * Rotation Matrix on Y-axis
         * |  cos -sin 0   0   |
         * |  sin  cos 0   0   |
         * |   0   0   1   0   |
         * |   0   0   0   1   |
         */
        matrix4x4 mat = matrix4x4::identity();
        mat[0][0] = mat[1][1] = std::cos(_angle);
        mat[0][1] = std::sin(_angle);
        mat[1][0] = -mat[0][1];

        return mat;
    }

    matrix4x4 matrix_util::rotation_matrix(const vector3& _euler_angles)
    {
        return rotation_matrix_x(_euler_angles.x_)*rotation_matrix_y(_euler_angles.y_)
                *rotation_matrix_z(_euler_angles.z_);

        // matrix4x4 rotationMatrix;
        // set_to_identity(rotationMatrix);

        // float r = maths_util::deg2rad * angle;
        // float c = std::cos(r);
        // float s = std::sin(r);
        // float omc = 1.0f - c;

        // float x = _euler_angles.x_;
        // float y = _euler_angles.y_;
        // float z = _euler_angles.z;

        // rotationMatrix[0][0] = x * omc + c;
        // rotationMatrix[0][1] = y * x * omc + z * s;
        // rotationMatrix[0][2] = x * z * omc - y * s;

        // rotationMatrix[1][0] = x * y * omc - z * s;
        // rotationMatrix[1][1] = y * omc + c;
        // rotationMatrix[1][2] = y * z * omc + x * s;

        // rotationMatrix[2][0] = x * z * omc + y * s;
        // rotationMatrix[2][1] = y * z * omc - x * s;
        // rotationMatrix[2][2] = z * omc + c;

        // return rotationMatrix;
    }

    matrix4x4 matrix_util::scale_matrix(const vector3& _scale)
    {
        matrix4x4 scaleMatrix;

        scaleMatrix[0][0] = _scale.x_;
        scaleMatrix[1][1] = _scale.y_;
        scaleMatrix[2][2] = _scale.z_;
        scaleMatrix[3][3] = 1.0f;

        return scaleMatrix;
    }

    matrix4x4
    matrix_util::model_matrix(const vector3& _translation, const vector3& _euler_angles, const vector3& _scale)
    {
        return translation_matrix(_translation)*rotation_matrix(_euler_angles)*scale_matrix(_scale);
    }

    matrix4x4 matrix_util::view_matrix(const vector3& _forward, const vector3& _up, const vector3& _position)
    {
        vector3 right = _forward.cross(_up);
        /**
         * translation * orientation matrix  = homogeneous matrix
         * [ 1 0 0 T1 ][ R11  R12  R13  0 ]    [ R11 R12 R13 T1 ]
         * [ 0 1 0 T2 ][ R21  R22  R23  0 ]  = [ R21 R22 R23 T2 ]
         * [ 0 0 1 T3 ][ R31  R32  R33  0 ]    [ R31 R32 R33 T3 ]
         * [ 0 0 0 1  ][  0    0    0   1 ]    [  0   0   0  1  ]
         *
         * [  Right   trans ]
         * [  Up      trans ]
         * [ -Forward trans ]
         * [  0   0   0   1  ]
         */
        matrix4x4 mat;
        mat[3][3] = 1.0f;

        // Column 0
        mat[0][0] = right.x_;
        mat[0][1] = _up.x_;
        mat[0][2] = -_forward.x_;

        // Column 1
        mat[1][0] = right.y_;
        mat[1][1] = _up.y_;
        mat[1][2] = -_forward.y_;

        // Column 2
        mat[2][0] = right.z_;
        mat[2][1] = _up.z_;
        mat[2][2] = -_forward.z_;

        mat[3][0] = _position.x_;
        mat[3][1] = _position.y_;
        mat[3][2] = _position.z_;

        return mat;
    }

    matrix4x4 matrix_util::perspective_matrix(float _aspect_ratio, float _fov, float _near_plane, float _far_plane)
    {
        matrix4x4 mat = matrix4x4::identity();

        /**
         * AR  = Aspect Ratio
         * FOV = Field of View
         * N   = Near Plane
         * F   = Far Plane
         *
         * Perspective Matrix:
         * | 1/(tan(FOV/2) * AR)         0                0              0       |
         * |          0            1/tan(FOV/2)           0              0       |
         * |          0                  0          (N+F)/(N-F)   (2*F*N)/(N-F)  |
         * |          0                  0               -1              0       |
         * */

        float b = 1.0f/std::tan(0.5f*_fov);
        float a = b/_aspect_ratio;
        float c = (_near_plane+_far_plane)/(_near_plane-_far_plane);
        float d = (2.0f*_near_plane*_far_plane)/(_near_plane-_far_plane);

        mat[0][0] = a;
        mat[1][1] = b;
        mat[2][2] = c;
        mat[2][3] = -1.0f;
        mat[3][2] = d;

        return mat;
    }

    matrix4x4 matrix_util::orthographic_matrix(float _left, float _right, float _top, float _bottom, float _near_plane,
            float _far_plane)
    {
        matrix4x4 mat = matrix4x4::identity();

        mat[0][0] = 2.0f/(_right-_left);
        mat[1][1] = 2.0f/(_top-_bottom);
        mat[2][2] = 2.0f/(_near_plane-_far_plane);

        mat[3][0] = (_right+_left)/(_left-_right);
        mat[3][1] = (_top+_bottom)/(_bottom-_top);
        mat[3][2] = (_far_plane+_near_plane)/(_near_plane-_far_plane);

        mat[3][3] = 1.0f;

        return mat;
    }

    matrix4x4
    matrix_util::orthographic_matrix(float _aspect_ratio, float _ortho_size, float _near_plane, float _far_plane)
    {
        // [https://en.wikipedia.org/wiki/Orthographic_projection]
        // Most tutorial take the absolute position of the viewing box in the world as the input.
        // So there is a need to translate the box back to the origin.
        // We do not need to do so as we do our calculation assuming that we are already at the origin.

        matrix4x4 mat = matrix4x4::identity();

        float top = _ortho_size*0.5f;
        float bottom = -top;
        float right = top*_aspect_ratio;
        float left = bottom*_aspect_ratio;

        mat[0][0] = 2.0f/(right-left);
        mat[1][1] = 2.0f/(top-bottom);
        mat[2][2] = 2.0f/(_far_plane-_near_plane);

        return mat;
    }

    matrix4x4 matrix_util::inverse_matrix(const matrix4x4& _matrix)
    {
        matrix4x4 mat;

        mat[0][0] =
                _matrix[0][5]*_matrix[0][10]*_matrix[0][15]-
                        _matrix[0][5]*_matrix[0][11]*_matrix[0][14]-
                        _matrix[0][9]*_matrix[0][6]*_matrix[0][15]+
                        _matrix[0][9]*_matrix[0][7]*_matrix[0][14]+
                        _matrix[0][13]*_matrix[0][6]*_matrix[0][11]-
                        _matrix[0][13]*_matrix[0][7]*_matrix[0][10];

        mat[0][4] =
                -_matrix[0][4]*_matrix[0][10]*_matrix[0][15]+
                        _matrix[0][4]*_matrix[0][11]*_matrix[0][14]+
                        _matrix[0][8]*_matrix[0][6]*_matrix[0][15]-
                        _matrix[0][8]*_matrix[0][7]*_matrix[0][14]-
                        _matrix[0][12]*_matrix[0][6]*_matrix[0][11]+
                        _matrix[0][12]*_matrix[0][7]*_matrix[0][10];

        mat[0][8] =
                _matrix[0][4]*_matrix[0][9]*_matrix[0][15]-
                        _matrix[0][4]*_matrix[0][11]*_matrix[0][13]-
                        _matrix[0][8]*_matrix[0][5]*_matrix[0][15]+
                        _matrix[0][8]*_matrix[0][7]*_matrix[0][13]+
                        _matrix[0][12]*_matrix[0][5]*_matrix[0][11]-
                        _matrix[0][12]*_matrix[0][7]*_matrix[0][9];

        mat[0][12] =
                -_matrix[0][4]*_matrix[0][9]*_matrix[0][14]+
                        _matrix[0][4]*_matrix[0][10]*_matrix[0][13]+
                        _matrix[0][8]*_matrix[0][5]*_matrix[0][14]-
                        _matrix[0][8]*_matrix[0][6]*_matrix[0][13]-
                        _matrix[0][12]*_matrix[0][5]*_matrix[0][10]+
                        _matrix[0][12]*_matrix[0][6]*_matrix[0][9];

        mat[0][1] =
                -_matrix[0][1]*_matrix[0][10]*_matrix[0][15]+
                        _matrix[0][1]*_matrix[0][11]*_matrix[0][14]+
                        _matrix[0][9]*_matrix[0][2]*_matrix[0][15]-
                        _matrix[0][9]*_matrix[0][3]*_matrix[0][14]-
                        _matrix[0][13]*_matrix[0][2]*_matrix[0][11]+
                        _matrix[0][13]*_matrix[0][3]*_matrix[0][10];

        mat[0][5] =
                _matrix[0][0]*_matrix[0][10]*_matrix[0][15]-
                        _matrix[0][0]*_matrix[0][11]*_matrix[0][14]-
                        _matrix[0][8]*_matrix[0][2]*_matrix[0][15]+
                        _matrix[0][8]*_matrix[0][3]*_matrix[0][14]+
                        _matrix[0][12]*_matrix[0][2]*_matrix[0][11]-
                        _matrix[0][12]*_matrix[0][3]*_matrix[0][10];

        mat[0][9] =
                -_matrix[0][0]*_matrix[0][9]*_matrix[0][15]+
                        _matrix[0][0]*_matrix[0][11]*_matrix[0][13]+
                        _matrix[0][8]*_matrix[0][1]*_matrix[0][15]-
                        _matrix[0][8]*_matrix[0][3]*_matrix[0][13]-
                        _matrix[0][12]*_matrix[0][1]*_matrix[0][11]+
                        _matrix[0][12]*_matrix[0][3]*_matrix[0][9];

        mat[0][13] =
                _matrix[0][0]*_matrix[0][9]*_matrix[0][14]-
                        _matrix[0][0]*_matrix[0][10]*_matrix[0][13]-
                        _matrix[0][8]*_matrix[0][1]*_matrix[0][14]+
                        _matrix[0][8]*_matrix[0][2]*_matrix[0][13]+
                        _matrix[0][12]*_matrix[0][1]*_matrix[0][10]-
                        _matrix[0][12]*_matrix[0][2]*_matrix[0][9];

        mat[0][2] =
                _matrix[0][1]*_matrix[0][6]*_matrix[0][15]-
                        _matrix[0][1]*_matrix[0][7]*_matrix[0][14]-
                        _matrix[0][5]*_matrix[0][2]*_matrix[0][15]+
                        _matrix[0][5]*_matrix[0][3]*_matrix[0][14]+
                        _matrix[0][13]*_matrix[0][2]*_matrix[0][7]-
                        _matrix[0][13]*_matrix[0][3]*_matrix[0][6];

        mat[0][6] =
                -_matrix[0][0]*_matrix[0][6]*_matrix[0][15]+
                        _matrix[0][0]*_matrix[0][7]*_matrix[0][14]+
                        _matrix[0][4]*_matrix[0][2]*_matrix[0][15]-
                        _matrix[0][4]*_matrix[0][3]*_matrix[0][14]-
                        _matrix[0][12]*_matrix[0][2]*_matrix[0][7]+
                        _matrix[0][12]*_matrix[0][3]*_matrix[0][6];

        mat[0][10] =
                _matrix[0][0]*_matrix[0][5]*_matrix[0][15]-
                        _matrix[0][0]*_matrix[0][7]*_matrix[0][13]-
                        _matrix[0][4]*_matrix[0][1]*_matrix[0][15]+
                        _matrix[0][4]*_matrix[0][3]*_matrix[0][13]+
                        _matrix[0][12]*_matrix[0][1]*_matrix[0][7]-
                        _matrix[0][12]*_matrix[0][3]*_matrix[0][5];

        mat[0][14] =
                -_matrix[0][0]*_matrix[0][5]*_matrix[0][14]+
                        _matrix[0][0]*_matrix[0][6]*_matrix[0][13]+
                        _matrix[0][4]*_matrix[0][1]*_matrix[0][14]-
                        _matrix[0][4]*_matrix[0][2]*_matrix[0][13]-
                        _matrix[0][12]*_matrix[0][1]*_matrix[0][6]+
                        _matrix[0][12]*_matrix[0][2]*_matrix[0][5];

        mat[0][3] =
                -_matrix[0][1]*_matrix[0][6]*_matrix[0][11]+
                        _matrix[0][1]*_matrix[0][7]*_matrix[0][10]+
                        _matrix[0][5]*_matrix[0][2]*_matrix[0][11]-
                        _matrix[0][5]*_matrix[0][3]*_matrix[0][10]-
                        _matrix[0][9]*_matrix[0][2]*_matrix[0][7]+
                        _matrix[0][9]*_matrix[0][3]*_matrix[0][6];

        mat[0][7] =
                _matrix[0][0]*_matrix[0][6]*_matrix[0][11]-
                        _matrix[0][0]*_matrix[0][7]*_matrix[0][10]-
                        _matrix[0][4]*_matrix[0][2]*_matrix[0][11]+
                        _matrix[0][4]*_matrix[0][3]*_matrix[0][10]+
                        _matrix[0][8]*_matrix[0][2]*_matrix[0][7]-
                        _matrix[0][8]*_matrix[0][3]*_matrix[0][6];

        mat[0][11] =
                -_matrix[0][0]*_matrix[0][5]*_matrix[0][11]+
                        _matrix[0][0]*_matrix[0][7]*_matrix[0][9]+
                        _matrix[0][4]*_matrix[0][1]*_matrix[0][11]-
                        _matrix[0][4]*_matrix[0][3]*_matrix[0][9]-
                        _matrix[0][8]*_matrix[0][1]*_matrix[0][7]+
                        _matrix[0][8]*_matrix[0][3]*_matrix[0][5];

        mat[0][15] =
                _matrix[0][0]*_matrix[0][5]*_matrix[0][10]-
                        _matrix[0][0]*_matrix[0][6]*_matrix[0][9]-
                        _matrix[0][4]*_matrix[0][1]*_matrix[0][10]+
                        _matrix[0][4]*_matrix[0][2]*_matrix[0][9]+
                        _matrix[0][8]*_matrix[0][1]*_matrix[0][6]-
                        _matrix[0][8]*_matrix[0][2]*_matrix[0][5];

        float determinant =
                _matrix[0][0]*mat[0][0]
                        +_matrix[0][1]*mat[0][4]
                        +_matrix[0][2]*mat[0][8]
                        +_matrix[0][3]*mat[0][12];
        determinant = 1.0f/determinant;

        for (size_t i = 0; i<16; ++i) {
            mat[0][i] *= determinant;
        }

        return mat;
    }
}