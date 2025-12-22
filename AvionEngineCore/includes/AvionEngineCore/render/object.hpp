#pragma once

#include <iostream>

#include "glm/glm.hpp"

struct Size {
    Size() = default;
    Size(glm::vec3 size): size(size) {}
    glm::vec3 size;
};

struct Position {
    Position() = default;
    Position(glm::vec3 position): position(position) {}
    glm::vec3 position;
};

std::ostream& operator<<(std::ostream& out, Size size);

std::ostream& operator<<(std::ostream& out, Position pos);

class Object {
public:
    Object() = default;
    Object(Position position, Size size);

    Object(const Object& object);
    Object(Object&& object);

    Object& operator=(const Object& object);
    Object& operator=(Object&& object);

    ~Object();

    Size GetSize() const;
    Position GetPosition() const;

private:
    Size size_;
    Position position_;
};