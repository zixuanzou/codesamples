#ifndef Simplex_hpp
#define Simplex_hpp

#include <array>

#include "Vector3f.hpp"

const std::size_t SIMPLEX_MAX_SIZE = 4;

class Simplex {
private:
    std::array<Vector3f, SIMPLEX_MAX_SIZE> m_Points;
    std::size_t m_Size;
    
public:
    Simplex();
    
    void PushFront(Vector3f point);
    void IncrementSize();
    std::size_t Size() const;
    
    Vector3f operator[](std::size_t index) const;
    Simplex& operator=(std::initializer_list<Vector3f> list);
    
};

#endif /* Simplex_hpp */
