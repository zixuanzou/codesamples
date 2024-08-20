#include "Vector3f.hpp"

Vector3f::Vector3f() : x(0.0f), y(0.0f), z(0.0f) {};

Vector3f::Vector3f(float x, float y, float z) : x(x), y(y), z(z) {};

float Vector3f::Magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vector3f Vector3f::Normalize() const {
    float magnitude = Magnitude();
    if (magnitude == 0) {
        return Vector3f(1.0f, 0.0f, 0.0f);
    }
    float invMag = 1.0f / magnitude;
    return Vector3f(x * invMag, y * invMag, z * invMag);
}

float Vector3f::Dot(const Vector3f& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vector3f Vector3f::Cross(const Vector3f& other) const {
    return Vector3f(y * other.z - z * other.y,
                    z * other.x - x * other.z,
                    x * other.y - y * other.x);
}

Vector3f Vector3f::operator+(const Vector3f& other) const {
    return Vector3f(x + other.x, y + other.y, z + other.z);
}

Vector3f Vector3f::operator-(const Vector3f& other) const {
    return Vector3f(x - other.x, y - other.y, z - other.z);
}

Vector3f Vector3f::operator+=(const Vector3f& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3f Vector3f::operator-=(const Vector3f& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vector3f Vector3f::operator*(float scalar) const {
    return Vector3f(x * scalar, y * scalar, z * scalar);
}

Vector3f Vector3f::operator/(float scalar) const {
    return Vector3f(x / scalar, y / scalar, z / scalar);
}
