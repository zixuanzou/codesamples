#ifndef ManifoldPoints_hpp
#define ManifoldPoints_hpp

#include "Vector3f.hpp"

class ManifoldPoints {
public:
    Vector3f A;
    Vector3f B;
    Vector3f Normal;
    float Depth;
    bool HasCollision;
    
    ManifoldPoints();
    ManifoldPoints(Vector3f normal, float depth);
    ManifoldPoints(Vector3f a, Vector3f b, Vector3f normal, float depth);
};

#endif /* ManifoldPoints_hpp */
