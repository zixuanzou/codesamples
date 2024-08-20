#ifndef Transform_hpp
#define Transform_hpp

#include "Vector3f.hpp"

struct Transform {
    Vector3f Position;
    Vector3f Scale;
    float Rotation;
    
    Transform(Vector3f pos, Vector3f s, float r) : Position(pos), Scale(s), Rotation(r) {}
};

#endif /* Transform_hpp */
