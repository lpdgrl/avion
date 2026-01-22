#include "../../../includes/AvionEngineCore/core/object.hpp"

namespace avion::core {

    Object::Object(int id, Position position, Size size, Color color, Color mixing_color)
        : id_(id)
        , position_(position)
        , size_(size)
        , color_(color)
        , mixing_color_(mixing_color) 
    {
            std::cout << "Object ctor " << position_ << '\n';
    }

    Object::Object(const Object& object): position_(object.position_), size_(object.size_), color_(object.color_) {
        std::cout << "Object ctor copy " << position_  << '\n';
    }
    Object::Object(Object&& object): position_(object.position_), size_(object.size_), color_(object.color_) {
        std::cout << "Object ctor move " << position_ <<  '\n';
    }

    Object& Object::operator=(const Object& object) { 
        if (this != &object) {
            position_ = object.position_;
            size_ = object.size_;
            color_ = object.color_;
        }
        std::cout << "Object copy assingment " << position_  <<  '\n';    
        return *this; 
    }
    Object& Object::operator=(Object&& object) {
        if (this != &object) {
            position_ = object.position_;
            size_ = object.size_;
            color_ = object.color_;
        }
        std::cout << "Object move assingment " << position_ << '\n';
        return *this; 
    }

    Object::~Object() {
        std::cout << "Object dtor " << position_  <<  '\n';
    }

    Position Object::GetPosition() const noexcept {
        return position_;
    }

    Size Object::GetSize() const noexcept {
        return size_; 
    }

    Color Object::GetColor() const noexcept {
        return color_;
    }

    Color Object::GetMixingColor() const noexcept {
        return mixing_color_;
    }

    ObjectParams Object::GetParams() const noexcept {
        ObjectParams params{
            .position = position_.position,
            .size = size_.size,
            .color = color_.color,
            .mixing_color = color_.color
        };
        return params;
    }

    int Object::GetId() const noexcept {
        return id_;
    }

    std::ostream& operator<<(std::ostream& out, Size size) {
        return out << size.size.x << ' ' << size.size.y << ' ' << size.size.z << '\n';
    }

    std::ostream& operator<<(std::ostream& out, Position pos) {
        return out << pos.position.x << ' ' << pos.position.y << ' ' << pos.position.z << '\n';
    }

    void Object::SetPosition(Position position) noexcept {
        position_ = position;
    }

    void Object::SetSize(Size size) noexcept {
        size_ = size;
    }

    void Object::SetColor(Color color) noexcept {
        color_ = color;
    }

    void Object::SetMixingColor(Color color) noexcept {
        mixing_color_ = color;
    }

    void Object::SetParams(Position position, Size size, Color color) {
        position_ = position;
        size_ = size;
        color_ = color;
    }

    Position::operator glm::vec3() const {
        return position;
    }

    Size::operator glm::vec3() const {
        return size;
    }

    Color::operator glm::vec3() const {
        return color;
    }

    // ObjectId::ObjectId(int id, Position position, Size size, Color color): id(id), object(position, size, color) {}

} // namespace avion::core