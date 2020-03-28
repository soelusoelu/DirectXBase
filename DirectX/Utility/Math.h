#pragma once

#include <cmath>
#include <memory.h>
#include <limits>

namespace Math {
    constexpr float PI = 3.1415926535f;
    constexpr float TwoPI = PI * 2.0f;
    constexpr float PiOver2 = PI / 2.0f;
    constexpr float infinity = std::numeric_limits<float>::infinity();
    constexpr float negInfinity = -std::numeric_limits<float>::infinity();
    constexpr float deg2Rad = PI / 180.f;

    inline float toRadians(float degrees) {
        return degrees * PI / 180.0f;
    }

    inline float toDegrees(float radians) {
        return radians * 180.0f / PI;
    }

    inline bool nearZero(float val, float epsilon = 0.001f) {
        return (fabs(val) <= epsilon);
    }

    template <typename T>
    T Max(const T& a, const T& b) {
        return (a < b ? b : a);
    }

    template <typename T>
    T Min(const T& a, const T& b) {
        return (a < b ? a : b);
    }

    template <typename T>
    T clamp(const T& value, const T& min, const T& max) {
        return Min(max, Max(min, value));
    }

    template <typename T>
    T pow(T value, int pow) {
        for (int i = 0; i < pow; i++) {
            value *= value;
        }
        return value;
    }

    inline float abs(float value) {
        return fabs(value);
    }

    inline float cos(float degree) {
        return cosf(degree * deg2Rad);
    }

    inline float sin(float degree) {
        return sinf(degree * deg2Rad);
    }

    inline float tan(float degree) {
        return tanf(degree * deg2Rad);
    }

    inline float asin(float value) {
        return asinf(value);
    }

    inline float acos(float value) {
        return acosf(value);
    }

    inline float atan2(float y, float x) {
        return atan2f(y, x);
    }

    inline float cot(float degree) {
        return 1.0f / tan(degree);
    }

    inline float lerp(float a, float b, float f) {
        return a + f * (b - a);
    }

    inline float sqrt(float value) {
        return sqrtf(value);
    }

    inline float fmod(float numer, float denom) {
        return fmod(numer, denom);
    }
}

// 2D Vector
class Vector2 {
public:
    float x;
    float y;

    Vector2() :
        x(0.f),
        y(0.f) {
    }

    explicit Vector2(float inX, float inY) :
        x(inX),
        y(inY) {
    }

    // Set both components in one line
    void set(float inX, float inY) {
        x = inX;
        y = inY;
    }

    Vector2 operator-() {
        return Vector2(-x, -y);
    }

    friend Vector2 operator-(const Vector2& vec) {
        return Vector2(-vec.x, -vec.y);
    }

    // Vector addition (a + b)
    friend Vector2 operator+(const Vector2& a, const Vector2& b) {
        return Vector2(a.x + b.x, a.y + b.y);
    }

    // Vector subtraction (a - b)
    friend Vector2 operator-(const Vector2& a, const Vector2& b) {
        return Vector2(a.x - b.x, a.y - b.y);
    }

    // Component-wise multiplication
    // (a.x * b.x, ...)
    friend Vector2 operator*(const Vector2& a, const Vector2& b) {
        return Vector2(a.x * b.x, a.y * b.y);
    }

    // Scalar multiplication
    friend Vector2 operator*(const Vector2& vec, float scalar) {
        return Vector2(vec.x * scalar, vec.y * scalar);
    }

    // Scalar multiplication
    friend Vector2 operator*(float scalar, const Vector2& vec) {
        return Vector2(vec.x * scalar, vec.y * scalar);
    }

    friend Vector2 operator/(const Vector2& vec, float scalar) {
        if (scalar == 0) {
            return vec;
        }
        return Vector2(vec.x / scalar, vec.y / scalar);
    }

    // Scalar *=
    Vector2& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // Vector +=
    Vector2& operator+=(const Vector2& right) {
        x += right.x;
        y += right.y;
        return *this;
    }

    // Vector -=
    Vector2& operator-=(const Vector2& right) {
        x -= right.x;
        y -= right.y;
        return *this;
    }

    // Length squared of vector
    float lengthSq() const {
        return (x * x + y * y);
    }

    // Length of vector
    float length() const {
        return (Math::sqrt(lengthSq()));
    }

    static float distance(const Vector2& a, const Vector2& b) {
        auto dist = a - b;
        return dist.length();
    }

    void clamp(const Vector2& min, const Vector2& max) {
        x = Math::clamp<float>(x, min.x, max.x);
        y = Math::clamp<float>(y, min.y, max.y);
    }

    static Vector2 clamp(const Vector2& value, const Vector2& min, const Vector2& max) {
        Vector2 temp = value;
        temp.x = Math::clamp<float>(temp.x, min.x, max.x);
        temp.y = Math::clamp<float>(temp.y, min.y, max.y);
        return temp;
    }

    // Normalize this vector
    void normalize() {
        float len = length();
        if (len == 0) {
            return;
        }
        x /= len;
        y /= len;
    }

    // Normalize the provided vector
    static Vector2 normalize(const Vector2& vec) {
        Vector2 temp = vec;
        temp.normalize();
        return temp;
    }

    // Dot product between two vectors (a dot b)
    static float dot(const Vector2& a, const Vector2& b) {
        return (a.x * b.x + a.y * b.y);
    }

    static float cross(const Vector2& a, const Vector2& b) {
        return (a.x * b.y - a.y * b.x);
    }

    // Lerp from A to B by f
    static Vector2 lerp(const Vector2& a, const Vector2& b, float f) {
        return Vector2(a + f * (b - a));
    }

    // Reflect V about (normalized) N
    static Vector2 reflect(const Vector2& v, const Vector2& n) {
        return v - 2.f * Vector2::dot(v, n) * n;
    }

    // Transform vector by matrix
    static Vector2 transform(const Vector2& vec, const class Matrix3& mat, float w = 1.0f);

    static const Vector2 zero;
    static const Vector2 right;
    static const Vector2 up;
    static const Vector2 left;
    static const Vector2 down;
    static const Vector2 one;
};

class Vector2INT {
public:
    int x;
    int y;

    Vector2INT() :
        x(0),
        y(0) {
    }

    explicit Vector2INT(int inX, int inY) :
        x(inX),
        y(inY) {
    }

    void set(int inX, int inY) {
        x = inX;
        y = inY;
    }

    friend Vector2INT operator-(const Vector2INT& vec) {
        return Vector2INT(-vec.x, -vec.y);
    }

    friend Vector2INT operator+(const Vector2INT& a, const Vector2INT& b) {
        return Vector2INT(a.x + b.x, a.y + b.y);
    }

    friend Vector2INT operator-(const Vector2INT& a, const Vector2INT& b) {
        return Vector2INT(a.x - b.x, a.y - b.y);
    }

    friend Vector2INT operator*(const Vector2INT& a, const Vector2INT& b) {
        return Vector2INT(a.x * b.x, a.y * b.y);
    }

    friend Vector2INT operator*(const Vector2INT& vec, int scalar) {
        return Vector2INT(vec.x * scalar, vec.y * scalar);
    }

    friend Vector2INT operator*(int scalar, const Vector2INT& vec) {
        return Vector2INT(vec.x * scalar, vec.y * scalar);
    }

    friend Vector2INT operator/(const Vector2INT& vec, int scalar) {
        return Vector2INT(vec.x / scalar, vec.y / scalar);
    }

    Vector2INT& operator*=(int scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2INT& operator+=(const Vector2INT& right) {
        x += right.x;
        y += right.y;
        return *this;
    }

    Vector2INT& operator-=(const Vector2INT& right) {
        x -= right.x;
        y -= right.y;
        return *this;
    }

    static const Vector2INT zero;
    static const Vector2INT one;
};

// 3D Vector
class Vector3 {
public:
    float x;
    float y;
    float z;

    Vector3() :
        x(0.f),
        y(0.f),
        z(0.f) {
    }

    explicit Vector3(float inX, float inY, float inZ) :
        x(inX),
        y(inY),
        z(inZ) {
    }

    Vector3(const Vector2& vec2, float inZ) :
        x(vec2.x),
        y(vec2.y),
        z(inZ) {
    }

    // Set all three components in one line
    void set(float inX, float inY, float inZ) {
        x = inX;
        y = inY;
        z = inZ;
    }

    Vector3& operator=(const Vector3& vec) {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        return *this;
    }

    Vector3 operator-() {
        return Vector3(-x, -y, -z);
    }

    // Vector addition (a + b)
    friend Vector3 operator+(const Vector3& a, const Vector3& b) {
        return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    // Vector subtraction (a - b)
    friend Vector3 operator-(const Vector3& a, const Vector3& b) {
        return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
    }

    // Component-wise multiplication
    friend Vector3 operator*(const Vector3& left, const Vector3& right) {
        return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
    }

    // Scalar multiplication
    friend Vector3 operator*(const Vector3& vec, float scalar) {
        return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
    }

    // Scalar multiplication
    friend Vector3 operator*(float scalar, const Vector3& vec) {
        return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
    }

    friend Vector3 operator/(const Vector3& vec, float scalar) {
        if (scalar == 0) {
            return vec;
        }
        return Vector3(vec.x / scalar, vec.y / scalar, vec.z / scalar);
    }

    // Scalar *=
    Vector3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3& operator*=(const Vector3& right) {
        x *= right.x;
        y *= right.y;
        z *= right.z;
        return *this;
    }

    // Vector +=
    Vector3& operator+=(const Vector3& right) {
        x += right.x;
        y += right.y;
        z += right.z;
        return *this;
    }

    // Vector -=
    Vector3& operator-=(const Vector3& right) {
        x -= right.x;
        y -= right.y;
        z -= right.z;
        return *this;
    }

    // Length squared of vector
    float lengthSq() const {
        return (x * x + y * y + z * z);
    }

    // Length of vector
    float length() const {
        return (Math::sqrt(lengthSq()));
    }

    static float distance(const Vector3& a, const Vector3& b) {
        auto dist = a - b;
        return dist.length();
    }

    void clamp(const Vector3& min, const Vector3& max) {
        x = Math::clamp<float>(x, min.x, max.x);
        y = Math::clamp<float>(y, min.y, max.y);
        z = Math::clamp<float>(z, min.z, max.z);
    }

    static Vector3 clamp(const Vector3& value, const Vector3& min, const Vector3& max) {
        Vector3 temp = value;
        temp.x = Math::clamp<float>(temp.x, min.x, max.x);
        temp.y = Math::clamp<float>(temp.y, min.y, max.y);
        temp.z = Math::clamp<float>(temp.z, min.z, max.z);
        return temp;
    }

    // Normalize this vector
    void normalize() {
        float len = length();
        if (len == 0) {
            return;
        }
        x /= len;
        y /= len;
        z /= len;
    }

    // Normalize the provided vector
    static Vector3 normalize(const Vector3& vec) {
        Vector3 temp = vec;
        temp.normalize();
        return temp;
    }

    // Dot product between two vectors (a dot b)
    static float dot(const Vector3& a, const Vector3& b) {
        return (a.x * b.x + a.y * b.y + a.z * b.z);
    }

    // Cross product between two vectors (a cross b)
    static Vector3 cross(const Vector3& a, const Vector3& b) {
        return Vector3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    // Lerp from A to B by f
    static Vector3 lerp(const Vector3& a, const Vector3& b, float f) {
        return Vector3(a + f * (b - a));
    }

    // Reflect V about (normalized) N
    static Vector3 reflect(const Vector3& v, const Vector3& n) {
        return v - 2.0f * Vector3::dot(v, n) * n;
    }

    static Vector3 transform(const Vector3& vec, const class Matrix4& mat, float w = 1.0f);
    // This will transform the vector and renormalize the w component
    static Vector3 transformWithPerspDiv(const Vector3& vec, const class Matrix4& mat, float w = 1.0f);

    // Transform a Vector3 by a quaternion
    static Vector3 transform(const Vector3& v, const class Quaternion& q);

    static const Vector3 zero;
    static const Vector3 right;
    static const Vector3 up;
    static const Vector3 forward;
    static const Vector3 left;
    static const Vector3 down;
    static const Vector3 back;
    static const Vector3 one;
    static const Vector3 negOne;
    static const Vector3 Infinity;
    static const Vector3 NegInfinity;
};

// 3x3 Matrix
class Matrix3 {
public:
    float mat[3][3];

    Matrix3() {
        *this = Matrix3::identity;
    }

    explicit Matrix3(float inMat[3][3]) {
        memcpy(mat, inMat, 9 * sizeof(float));
    }

    // Cast to a const float pointer
    const float* getAsFloatPtr() const {
        return reinterpret_cast<const float*>(&mat[0][0]);
    }

    // Matrix multiplication
    friend Matrix3 operator*(const Matrix3& left, const Matrix3& right) {
        Matrix3 retVal;
        // row 0
        retVal.mat[0][0] =
            left.mat[0][0] * right.mat[0][0] +
            left.mat[0][1] * right.mat[1][0] +
            left.mat[0][2] * right.mat[2][0];

        retVal.mat[0][1] =
            left.mat[0][0] * right.mat[0][1] +
            left.mat[0][1] * right.mat[1][1] +
            left.mat[0][2] * right.mat[2][1];

        retVal.mat[0][2] =
            left.mat[0][0] * right.mat[0][2] +
            left.mat[0][1] * right.mat[1][2] +
            left.mat[0][2] * right.mat[2][2];

        // row 1
        retVal.mat[1][0] =
            left.mat[1][0] * right.mat[0][0] +
            left.mat[1][1] * right.mat[1][0] +
            left.mat[1][2] * right.mat[2][0];

        retVal.mat[1][1] =
            left.mat[1][0] * right.mat[0][1] +
            left.mat[1][1] * right.mat[1][1] +
            left.mat[1][2] * right.mat[2][1];

        retVal.mat[1][2] =
            left.mat[1][0] * right.mat[0][2] +
            left.mat[1][1] * right.mat[1][2] +
            left.mat[1][2] * right.mat[2][2];

        // row 2
        retVal.mat[2][0] =
            left.mat[2][0] * right.mat[0][0] +
            left.mat[2][1] * right.mat[1][0] +
            left.mat[2][2] * right.mat[2][0];

        retVal.mat[2][1] =
            left.mat[2][0] * right.mat[0][1] +
            left.mat[2][1] * right.mat[1][1] +
            left.mat[2][2] * right.mat[2][1];

        retVal.mat[2][2] =
            left.mat[2][0] * right.mat[0][2] +
            left.mat[2][1] * right.mat[1][2] +
            left.mat[2][2] * right.mat[2][2];

        return retVal;
    }

    Matrix3& operator*=(const Matrix3& right) {
        *this = *this * right;
        return *this;
    }

    // Create a scale matrix with x and y scales
    static Matrix3 createScale(float xScale, float yScale) {
        float temp[3][3] =
        {
            { xScale, 0.0f, 0.0f },
            { 0.0f, yScale, 0.0f },
            { 0.0f, 0.0f, 1.0f },
        };
        return Matrix3(temp);
    }

    static Matrix3 createScale(const Vector2& scaleVector) {
        return createScale(scaleVector.x, scaleVector.y);
    }

    // Create a scale matrix with a uniform factor
    static Matrix3 createScale(float scale) {
        return createScale(scale, scale);
    }

    // Create a rotation matrix about the Z axis
    // theta is in radians
    static Matrix3 createRotation(float theta) {
        float temp[3][3] =
        {
            { Math::cos(theta), Math::sin(theta), 0.0f },
            { -Math::sin(theta), Math::cos(theta), 0.0f },
            { 0.0f, 0.0f, 1.0f },
        };
        return Matrix3(temp);
    }

    // Create a translation matrix (on the xy-plane)
    static Matrix3 createTranslation(const Vector2& trans) {
        float temp[3][3] =
        {
            { 1.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
            { trans.x, trans.y, 1.0f },
        };
        return Matrix3(temp);
    }

    static const Matrix3 identity;
};

// 4x4 Matrix
class Matrix4 {
public:
    float m[4][4];

    Matrix4() {
        *this = Matrix4::identity;
    }

    explicit Matrix4(float inMat[4][4]) {
        memcpy(m, inMat, 16 * sizeof(float));
    }

    // Cast to a const float pointer
    const float* getAsFloatPtr() const {
        return reinterpret_cast<const float*>(&m[0][0]);
    }

    // Matrix multiplication (a * b)
    friend Matrix4 operator*(const Matrix4& a, const Matrix4& b) {
        Matrix4 retVal;
        // row 0
        retVal.m[0][0] =
            a.m[0][0] * b.m[0][0] +
            a.m[0][1] * b.m[1][0] +
            a.m[0][2] * b.m[2][0] +
            a.m[0][3] * b.m[3][0];

        retVal.m[0][1] =
            a.m[0][0] * b.m[0][1] +
            a.m[0][1] * b.m[1][1] +
            a.m[0][2] * b.m[2][1] +
            a.m[0][3] * b.m[3][1];

        retVal.m[0][2] =
            a.m[0][0] * b.m[0][2] +
            a.m[0][1] * b.m[1][2] +
            a.m[0][2] * b.m[2][2] +
            a.m[0][3] * b.m[3][2];

        retVal.m[0][3] =
            a.m[0][0] * b.m[0][3] +
            a.m[0][1] * b.m[1][3] +
            a.m[0][2] * b.m[2][3] +
            a.m[0][3] * b.m[3][3];

        // row 1
        retVal.m[1][0] =
            a.m[1][0] * b.m[0][0] +
            a.m[1][1] * b.m[1][0] +
            a.m[1][2] * b.m[2][0] +
            a.m[1][3] * b.m[3][0];

        retVal.m[1][1] =
            a.m[1][0] * b.m[0][1] +
            a.m[1][1] * b.m[1][1] +
            a.m[1][2] * b.m[2][1] +
            a.m[1][3] * b.m[3][1];

        retVal.m[1][2] =
            a.m[1][0] * b.m[0][2] +
            a.m[1][1] * b.m[1][2] +
            a.m[1][2] * b.m[2][2] +
            a.m[1][3] * b.m[3][2];

        retVal.m[1][3] =
            a.m[1][0] * b.m[0][3] +
            a.m[1][1] * b.m[1][3] +
            a.m[1][2] * b.m[2][3] +
            a.m[1][3] * b.m[3][3];

        // row 2
        retVal.m[2][0] =
            a.m[2][0] * b.m[0][0] +
            a.m[2][1] * b.m[1][0] +
            a.m[2][2] * b.m[2][0] +
            a.m[2][3] * b.m[3][0];

        retVal.m[2][1] =
            a.m[2][0] * b.m[0][1] +
            a.m[2][1] * b.m[1][1] +
            a.m[2][2] * b.m[2][1] +
            a.m[2][3] * b.m[3][1];

        retVal.m[2][2] =
            a.m[2][0] * b.m[0][2] +
            a.m[2][1] * b.m[1][2] +
            a.m[2][2] * b.m[2][2] +
            a.m[2][3] * b.m[3][2];

        retVal.m[2][3] =
            a.m[2][0] * b.m[0][3] +
            a.m[2][1] * b.m[1][3] +
            a.m[2][2] * b.m[2][3] +
            a.m[2][3] * b.m[3][3];

        // row 3
        retVal.m[3][0] =
            a.m[3][0] * b.m[0][0] +
            a.m[3][1] * b.m[1][0] +
            a.m[3][2] * b.m[2][0] +
            a.m[3][3] * b.m[3][0];

        retVal.m[3][1] =
            a.m[3][0] * b.m[0][1] +
            a.m[3][1] * b.m[1][1] +
            a.m[3][2] * b.m[2][1] +
            a.m[3][3] * b.m[3][1];

        retVal.m[3][2] =
            a.m[3][0] * b.m[0][2] +
            a.m[3][1] * b.m[1][2] +
            a.m[3][2] * b.m[2][2] +
            a.m[3][3] * b.m[3][2];

        retVal.m[3][3] =
            a.m[3][0] * b.m[0][3] +
            a.m[3][1] * b.m[1][3] +
            a.m[3][2] * b.m[2][3] +
            a.m[3][3] * b.m[3][3];

        return retVal;
    }

    Matrix4& operator*=(const Matrix4& right) {
        *this = *this * right;
        return *this;
    }

    void transpose();

    // Invert the matrix - super slow
    void invert();

    // Get the translation component of the matrix
    Vector3 getTranslation() const {
        return Vector3(m[3][0], m[3][1], m[3][2]);
    }

    // Get the X axis of the matrix (forward)
    Vector3 getXAxis() const {
        return Vector3::normalize(Vector3(m[0][0], m[0][1], m[0][2]));
    }

    // Get the Y axis of the matrix (left)
    Vector3 getYAxis() const {
        return Vector3::normalize(Vector3(m[1][0], m[1][1], m[1][2]));
    }

    // Get the Z axis of the matrix (up)
    Vector3 getZAxis() const {
        return Vector3::normalize(Vector3(m[2][0], m[2][1], m[2][2]));
    }

    // Extract the scale component from the matrix
    Vector3 getScale() const {
        Vector3 retVal;
        retVal.x = Vector3(m[0][0], m[0][1], m[0][2]).length();
        retVal.y = Vector3(m[1][0], m[1][1], m[1][2]).length();
        retVal.z = Vector3(m[2][0], m[2][1], m[2][2]).length();
        return retVal;
    }

    // Create a scale matrix with x, y, and z scales
    static Matrix4 createScale(float xScale, float yScale, float zScale) {
        float temp[4][4] =
        {
            { xScale, 0.0f, 0.0f, 0.0f },
            { 0.0f, yScale, 0.0f, 0.0f },
            { 0.0f, 0.0f, zScale, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f }
        };
        return Matrix4(temp);
    }

    static Matrix4 createScale(const Vector3& scaleVector) {
        return createScale(scaleVector.x, scaleVector.y, scaleVector.z);
    }

    // Create a scale matrix with a uniform factor
    static Matrix4 createScale(float scale) {
        return createScale(scale, scale, scale);
    }

    // Rotation about x-axis
    static Matrix4 createRotationX(float theta) {
        float temp[4][4] =
        {
            { 1.0f, 0.0f, 0.0f , 0.0f },
            { 0.0f, Math::cos(theta), Math::sin(theta), 0.0f },
            { 0.0f, -Math::sin(theta), Math::cos(theta), 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
        return Matrix4(temp);
    }

    // Rotation about y-axis
    static Matrix4 createRotationY(float theta) {
        float temp[4][4] =
        {
            { Math::cos(theta), 0.0f, -Math::sin(theta), 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            { Math::sin(theta), 0.0f, Math::cos(theta), 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
        return Matrix4(temp);
    }

    // Rotation about z-axis
    static Matrix4 createRotationZ(float theta) {
        float temp[4][4] =
        {
            { Math::cos(theta), Math::sin(theta), 0.0f, 0.0f },
            { -Math::sin(theta), Math::cos(theta), 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
        return Matrix4(temp);
    }

    // Create a rotation matrix from a quaternion
    static Matrix4 createFromQuaternion(const class Quaternion& q);

    static Matrix4 createTranslation(const Vector3& trans) {
        float temp[4][4] =
        {
            { 1.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { trans.x, trans.y, trans.z, 1.0f }
        };
        return Matrix4(temp);
    }

    static Matrix4 createOrtho(float width, float height, float _near, float _far) {
        float temp[4][4] =
        {
            { 2.0f / width, 0.0f, 0.0f, 0.0f },
            { 0.0f, -2.0f / height, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f / (_far - _near), 0.0f },
            { -1.0f, 1.0f, _near / (_near - _far), 1.0f }
        };
        return Matrix4(temp);
    }

    // Create "Simple" View-Projection Matrix from Chapter 6
    static Matrix4 createSimpleViewProj(float width, float height) {
        float temp[4][4] =
        {
            { 2.0f / width, 0.0f, 0.0f, 0.0f },
            { 0.0f, 2.0f / height, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 1.0f }
        };
        return Matrix4(temp);
    }

    static const Matrix4 identity;
};

// (Unit) Quaternion
class Quaternion {
public:
    float x;
    float y;
    float z;
    float w;

    Quaternion() {
        *this = Quaternion::identity;
    }

    // This directly sets the quaternion components --
    // don't use for axis/angle
    explicit Quaternion(float inX, float inY, float inZ, float inW) {
        set(inX, inY, inZ, inW);
    }

    // Construct the quaternion from an axis and angle
    // It is assumed that axis is already normalized,
    // and the angle is in radians
    explicit Quaternion(const Vector3& axis, float angle) {
        float scalar = Math::sin(angle / 2.0f);
        x = axis.x * scalar;
        y = axis.y * scalar;
        z = axis.z * scalar;
        w = Math::cos(angle / 2.0f);
    }

    // Directly set the internal components
    void set(float inX, float inY, float inZ, float inW) {
        x = inX;
        y = inY;
        z = inZ;
        w = inW;
    }

    void conjugate() {
        x *= -1.0f;
        y *= -1.0f;
        z *= -1.0f;
    }

    float lengthSq() const {
        return (x * x + y * y + z * z + w * w);
    }

    float length() const {
        return Math::sqrt(lengthSq());
    }

    void normalize() {
        float len = length();
        if (len == 0) {
            return;
        }
        x /= len;
        y /= len;
        z /= len;
        w /= len;
    }

    // Normalize the provided quaternion
    static Quaternion normalize(const Quaternion& q) {
        Quaternion retVal = q;
        retVal.normalize();
        return retVal;
    }

    // Linear interpolation
    static Quaternion lerp(const Quaternion& a, const Quaternion& b, float f) {
        Quaternion retVal;
        retVal.x = Math::lerp(a.x, b.x, f);
        retVal.y = Math::lerp(a.y, b.y, f);
        retVal.z = Math::lerp(a.z, b.z, f);
        retVal.w = Math::lerp(a.w, b.w, f);
        retVal.normalize();
        return retVal;
    }

    static float dot(const Quaternion& a, const Quaternion& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    // Spherical Linear Interpolation
    static Quaternion slerp(const Quaternion& a, const Quaternion& b, float f) {
        float rawCosm = Quaternion::dot(a, b);

        float cosom = -rawCosm;
        if (rawCosm >= 0.0f) {
            cosom = rawCosm;
        }

        float scale0, scale1;

        if (cosom < 0.9999f) {
            const float omega = Math::acos(cosom);
            const float invSin = 1.f / Math::sin(omega);
            scale0 = Math::sin((1.f - f) * omega) * invSin;
            scale1 = Math::sin(f * omega) * invSin;
        } else {
            // Use linear interpolation if the quaternions
            // are collinear
            scale0 = 1.0f - f;
            scale1 = f;
        }

        if (rawCosm < 0.0f) {
            scale1 = -scale1;
        }

        Quaternion retVal;
        retVal.x = scale0 * a.x + scale1 * b.x;
        retVal.y = scale0 * a.y + scale1 * b.y;
        retVal.z = scale0 * a.z + scale1 * b.z;
        retVal.w = scale0 * a.w + scale1 * b.w;
        retVal.normalize();
        return retVal;
    }

    // Concatenate
    // Rotate by q FOLLOWED BY p
    //掛け算
    static Quaternion concatenate(const Quaternion& q, const Quaternion& p) {
        Quaternion retVal;

        // Vector component is:
        // ps * qv + qs * pv + pv x qv
        Vector3 qv(q.x, q.y, q.z);
        Vector3 pv(p.x, p.y, p.z);
        Vector3 newVec = p.w * qv + q.w * pv + Vector3::cross(pv, qv);
        retVal.x = newVec.x;
        retVal.y = newVec.y;
        retVal.z = newVec.z;

        // Scalar component is:
        // ps * qs - pv . qv
        retVal.w = p.w * q.w - Vector3::dot(pv, qv);

        return retVal;
    }

    static const Quaternion identity;
};

class Vector4 {
public:
    float x;
    float y;
    float z;
    float w;

    Vector4() :
        x(0.f),
        y(0.f),
        z(0.f),
        w(0.f) {
    }

    explicit Vector4(float inX, float inY, float inZ, float inW) :
        x(inX),
        y(inY),
        z(inZ),
        w(inW) {
    }

    Vector4(const Vector3& vec3, float inW) :
        x(vec3.x),
        y(vec3.y),
        z(vec3.z),
        w(inW) {
    }

    Vector4& operator=(const Vector4& vec) {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        w = vec.w;
        return *this;
    }
};

namespace ColorPalette {
    static const Vector3 black(0.0f, 0.0f, 0.0f);
    static const Vector3 white(1.0f, 1.0f, 1.0f);
    static const Vector3 red(1.0f, 0.0f, 0.0f);
    static const Vector3 green(0.0f, 1.0f, 0.0f);
    static const Vector3 blue(0.0f, 0.0f, 1.0f);
    static const Vector3 yellow(1.0f, 1.0f, 0.0f);
    static const Vector3 lightYellow(1.0f, 1.0f, 0.88f);
    static const Vector3 lightBlue(0.68f, 0.85f, 0.9f);
    static const Vector3 lightPink(1.0f, 0.71f, 0.76f);
    static const Vector3 lightGreen(0.56f, 0.93f, 0.56f);
}
