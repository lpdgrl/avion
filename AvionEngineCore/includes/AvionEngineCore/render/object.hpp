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

struct Color {
    Color() = default;
    Color(glm::vec3 color): color(color) {}
    glm::vec3 color;
};

std::ostream& operator<<(std::ostream& out, Size size);

std::ostream& operator<<(std::ostream& out, Position pos);

class Object {
public:
    Object() = default;
    Object(Position position, Size size, Color color);

    Object(const Object& object);
    Object(Object&& object);

    Object& operator=(const Object& object);
    Object& operator=(Object&& object);

    ~Object();

    Size GetSize() const;
    Position GetPosition() const;
    Color GetColor() const;

private:
    Size size_;
    Position position_;
    Color color_;
};