#include "../../../includes/AvionEngineCore/core/object.hpp"

namespace avion::core {

    Object::Object(int id, gfx::Transform transform, Color color, Color mixing_color, Material material)
        : id_(id)
        , m_transform(transform)
        , color_(color)
        , mixing_color_(mixing_color) 
        , material_(material)
    {
        AV_LOG_INFO("Object ctor " + std::to_string(m_transform.position.x) + " " + std::to_string(m_transform.position.y) + " " + std::to_string(m_transform.position.z));
    }

    Object::Object(int id, ObjectParams params) 
        : id_(id)
        , m_transform(params.transform)
        , color_(params.color)
        , mixing_color_(params.mixing_color)
        , material_(params.material)
    {
        AV_LOG_INFO("Object ctor with args ObjectParams");
    }

    // TODO: Object copy-ctor isn't actually!!
    Object::Object(const Object& object): m_transform(object.m_transform),  color_(object.color_) {
        std::cout << "Object ctor copy " <<  '\n';
    }
    
    // TODO: Object move-ctor isn't actually!!
    Object::Object(Object&& object): m_transform(object.m_transform), color_(object.color_) {
        std::cout << "Object ctor move "  <<  '\n';
    }

    Object& Object::operator=(const Object& object) { 
        if (this != &object) {
            m_transform = object.m_transform;
            color_ = object.color_;
            mixing_color_ = object.mixing_color_;
            material_ = object.material_;
        }
        std::cout << "Object copy assingment " <<  id_ << '\n';    
        return *this; 
    }

    Object& Object::operator=(Object&& object) {
        if (this != &object) {
            id_ = object.id_;
            m_transform = object.m_transform;
            color_ = object.color_;
            mixing_color_ = object.mixing_color_;
            material_ = object.material_;
        }
        std::cout << "Object move assingment " << '\n';
        return *this; 
    }

    Object::~Object() {
        std::cout << "Object dtor " <<  id_ <<  '\n';
    }


    const Color& Object::GetColor() const noexcept {
        return color_;
    }

    Color& Object::GetColor() noexcept 
    {
      return color_;
    }

    Color Object::GetMixingColor() const noexcept {
        return mixing_color_;
    }

    ObjectParams Object::GetParams() const noexcept {
        ObjectParams params{
            .transform = m_transform,
            .color = color_.color,
            .mixing_color = color_.color,
            .material = material_
        };
        return params;
    }

    int Object::GetId() const noexcept {
        return id_;
    }

    void Object::SetParams(ObjectParams params) noexcept {
        m_transform = params.transform;
        color_ = params.color;
        mixing_color_ = params.mixing_color;
        material_ = params.material;
    }


    gfx::Transform& Object::GetTransform() noexcept 
    {
      return m_transform;
    }

    const gfx::Transform& Object::GetTransform() const noexcept
    {
      return m_transform;
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

    const Material& Object::GetMaterial() const noexcept
    {
      return material_;
    }

    Material& Object::GetMaterial() noexcept
    {
      return material_;
    }

    // ObjectId::ObjectId(int id, Position position, Size size, Color color): id(id), object(position, size, color) {}

} // namespace avion::core
