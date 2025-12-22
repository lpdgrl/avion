#include "../../../includes/AvionEngineCore/render/object.hpp"

Object::Object(Position position, Size size): position_(position), size_(size) {
    std::cout << "Object ctor " << position_ << '\n';
}

Object::Object(const Object& object): position_(object.position_), size_(object.size_) {
    std::cout << "Object ctor copy " << position_  << '\n';
}
Object::Object(Object&& object): position_(object.position_), size_(object.size_) {
    std::cout << "Object ctor move " << position_ <<  '\n';
}

Object& Object::operator=(const Object& object) { 
    if (this != &object) {
        position_ = object.position_;
        size_ = object.size_;
    }
    std::cout << "Object copy assingment " << position_  <<  '\n';    
    return *this; 
}
Object& Object::operator=(Object&& object) {
    if (this != &object) {
        position_ = object.position_;
        size_ = object.size_;
    }
    std::cout << "Object move assingment " << position_ << '\n';
    return *this; 
}

Object::~Object() {
    std::cout << "Object dtor " << position_  <<  '\n';
}

Position Object::GetPosition() const {
    return position_;
}

Size Object::GetSize() const {
    return size_; 
}

std::ostream& operator<<(std::ostream& out, Size size) {
    return out << size.size.x << ' ' << size.size.y << ' ' << size.size.z;
}

std::ostream& operator<<(std::ostream& out, Position pos) {
    return out << pos.position.x << ' ' << pos.position.y << ' ' << pos.position.z;
}
