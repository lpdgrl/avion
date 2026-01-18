#pragma once

#include <iostream>

#include "glm/glm.hpp"

namespace avion::core {

    struct Size {
        Size() = default;
        Size(glm::vec3 size): size(size) {}

        explicit operator glm::vec3() const;
        glm::vec3 size;
    };

    struct Position {
        Position() = default;
        Position(glm::vec3 position): position(position) {}

        explicit operator glm::vec3() const;
        glm::vec3 position;
    };

    struct Color {
        Color() = default;
        Color(glm::vec3 color): color(color) {}

        explicit operator glm::vec3() const;
        glm::vec3 color;
    };

    struct ObjectParams {
        glm::vec3 position;
        glm::vec3 size;
        glm::vec3 color;
    };

    std::ostream& operator<<(std::ostream& out, Size size);

    std::ostream& operator<<(std::ostream& out, Position pos);

    class Object {
    public:
        Object() = default;
        Object(int id, Position position, Size size, Color color);

        Object(const Object& object);
        Object(Object&& object);

        Object& operator=(const Object& object);
        Object& operator=(Object&& object);

        ~Object();

        Size GetSize() const noexcept;
        Position GetPosition() const noexcept;
        Color GetColor() const noexcept;

        ObjectParams GetParams() const noexcept;
        int GetId() const noexcept;

        void SetPosition(Position position);
        void SetSize(Size size);
        void SetColor(Color color);
        void SetParams(Position position, Size size, Color color);

    private:
        int id_ = 0;
        Size size_;
        Position position_;
        Color color_;
    };

    // struct ObjectId {
    //     ObjectId(int id, Position position, Size size, Color color);
    //     int id = 0;
    //     Object object;
    // };

} // namespace avion::core