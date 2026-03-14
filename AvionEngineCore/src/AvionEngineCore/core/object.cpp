#include "../../../includes/AvionEngineCore/core/object.hpp"

namespace avion::core {

    Object::Object(int id, Position position, Size size, Color color, Color mixing_color, Material material)
        : id_(id)
        , size_(size)
        , position_(position)
        , color_(color)
        , mixing_color_(mixing_color) 
        , material_(material)
    {
        AV_LOG_INFO("Object ctor " + std::to_string(position_.position.x) + " " + std::to_string(position_.position.y) + " " + std::to_string(position_.position.z));
    }

    Object::Object(int id, ObjectParams params) 
        : id_(id)
        , size_(params.size)
        , position_(params.position)
        , color_(params.color)
        , mixing_color_(params.mixing_color)
        , material_(params.material)
    {
        AV_LOG_INFO("Object ctor with args ObjectParams");
    }

    // TODO: Object copy-ctor isn't actually!!
    Object::Object(const Object& object): position_(object.position_), size_(object.size_), color_(object.color_) {
        std::cout << "Object ctor copy " << position_  << '\n';
    }
    
    // TODO: Object move-ctor isn't actually!!
    Object::Object(Object&& object): position_(object.position_), size_(object.size_), color_(object.color_) {
        std::cout << "Object ctor move " << position_ <<  '\n';
    }

    Object& Object::operator=(const Object& object) { 
        if (this != &object) {
            position_ = object.position_;
            size_ = object.size_;
            color_ = object.color_;
            mixing_color_ = object.mixing_color_;
            material_ = object.material_;
        }
        std::cout << "Object copy assingment " << position_  <<  '\n';    
        return *this; 
    }

    Object& Object::operator=(Object&& object) {
        if (this != &object) {
            position_ = object.position_;
            size_ = object.size_;
            color_ = object.color_;
            mixing_color_ = object.mixing_color_;
            material_ = object.material_;
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
            .mixing_color = color_.color,
            .material = material_
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

    void Object::SetParams(ObjectParams params) noexcept {
        position_ = params.position;
        size_ = params.size;
        color_ = params.color;
        mixing_color_ = params.mixing_color;
        material_ = params.material;
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
