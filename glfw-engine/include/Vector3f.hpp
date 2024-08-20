#ifndef Vector3f_hpp
#define Vector3f_hpp

#include <cmath>

class Vector3f {
public:
    float x;
    float y;
    float z;
    
    Vector3f();
    Vector3f(float x, float y, float z);
    
    float Magnitude() const;
    Vector3f Normalize() const;
    float Dot(const Vector3f& other) const;
    Vector3f Cross(const Vector3f& other) const;
    
    Vector3f operator+(const Vector3f& other) const;
    Vector3f operator-(const Vector3f& other) const;
    Vector3f operator+=(const Vector3f& other);
    Vector3f operator-=(const Vector3f& other);
    
    Vector3f operator*(float scalar) const;
    Vector3f operator/(float scalar) const;
};

#endif /* Vector3f_hpp */
