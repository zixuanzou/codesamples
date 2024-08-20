#include "Simplex.hpp"

Simplex::Simplex() {
    m_Points = {Vector3f(), Vector3f(), Vector3f(), Vector3f()};
    m_Size = 0;
}

void Simplex::PushFront(Vector3f point) {
    m_Points = {point, m_Points[0], m_Points[1], m_Points[2]};
    IncrementSize();
}

void Simplex::IncrementSize() {
    m_Size = std::min(m_Size + 1, SIMPLEX_MAX_SIZE);
}

std::size_t Simplex::Size() const {
    return m_Size;
}

Vector3f Simplex::operator[](std::size_t index) const {
    return m_Points[index];
}

Simplex& Simplex::operator=(std::initializer_list<Vector3f> list) {
    for (auto it = list.begin(); it != list.end(); it++) {
        m_Points[std::distance(list.begin(), it)] = *it;
    }
    m_Size = list.size();
    return *this;
}
