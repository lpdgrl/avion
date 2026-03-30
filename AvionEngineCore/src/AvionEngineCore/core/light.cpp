#include "../../includes/AvionEngineCore/core/light.hpp"

namespace avion::core {
    glm::vec3 ILight::GetAmbient() const {
        return GetAmbientImpl();
    }

    glm::vec3 ILight::GetDiffuse() const {
        return GetDiffuseImpl();
    }

    glm::vec3 ILight::GetSpecular() const {
        return GetSpecularImpl();
    }

    void ILight::SetAmbient(glm::vec3 ambient) {
        SetAmbientImpl(ambient);
    }

    void ILight::SetDiffuse(glm::vec3 diffuse) {
        SetDiffuseImpl(diffuse);
    }

    void ILight::SetSpecular(glm::vec3 specular) {
        SetSpecularImpl(specular);
    }

    void ILight::SetParams() {
       SetParamsImpl();
    }

    glm::vec3& ILight::GetGeometry() {
        return GetGeometryImpl();
    }

    DirLight::DirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : direction_(direction)
    , ambient_(ambient)
    , diffuse_(diffuse)
    , specular_(specular)
    {
    
    }
    
    glm::vec3 DirLight::GetAmbientImpl() const {
        return ambient_;
    }    

    glm::vec3 DirLight::GetDiffuseImpl() const {
        return diffuse_;
    }

    glm::vec3 DirLight::GetSpecularImpl() const {
        return specular_;
    }

    void DirLight::SetAmbientImpl(glm::vec3 ambient) {
        ambient_ = ambient;
    }

    void DirLight::SetDiffuseImpl(glm::vec3 diffuse) {
        diffuse_ = diffuse;
    } 

    void DirLight::SetSpecularImpl(glm::vec3 specular) {
        specular_ = specular;
    }

    void DirLight::SetParamsImpl() {

    }

    glm::vec3& DirLight::GetGeometryImpl() {
        return direction_;
    }

    void DirLight::SetDirection(glm::vec3 direction) noexcept {
        direction_ = direction;
    }

    glm::vec3 DirLight::GetDirection() const noexcept {
        return direction_;
    }

    DirLight::~DirLight() 
    {
        AV_LOG_DEBUG("Directional light is destroyed");
    }

    PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,
            glm::vec3 specular, float constant, float linear, float quadratic)
    : position_(position)
    , ambient_(ambient)
    , diffuse_(diffuse)
    , specular_(specular)
    , constant_(constant)
    , linear_(linear)
    , quadratic_(quadratic)
    {

    }

    glm::vec3 PointLight::GetAmbientImpl() const {
        return ambient_;
    }

    glm::vec3 PointLight::GetDiffuseImpl() const {
        return diffuse_;
    }

    glm::vec3 PointLight::GetSpecularImpl() const {
        return specular_;
    }

    void PointLight::SetAmbientImpl(glm::vec3 ambient) {
        ambient_ = ambient;
    }

    void PointLight::SetDiffuseImpl(glm::vec3 diffuse) {
        diffuse_ = diffuse;
    }

    void PointLight::SetSpecularImpl(glm::vec3 specular) {
        specular_ = specular;
    }

    void PointLight::SetParamsImpl() {

    }

    glm::vec3& PointLight::GetGeometryImpl() {
        return position_;
    }

    void PointLight::SetPosition(glm::vec3 position) noexcept {
        position_ = position;
    }

    glm::vec3 PointLight::GetPosition() const noexcept {
        return position_;
    }
    
    void PointLight::SetConstant(float constant) noexcept {
        constant_ = constant;
    }

    void PointLight::SetLinear(float linear) noexcept {
        linear_ = linear;
    }

    void PointLight::SetQuadratic(float quadratic) noexcept {
        quadratic_ = quadratic;
    }

    float PointLight::GetConstant() const noexcept {
        return constant_;
    }
    
    float PointLight::GetLinear() const noexcept {
        return linear_;
    }

    float PointLight::GetQuadratic() const noexcept {
        return quadratic_;
    }

    PointLight::~PointLight() 
    {
        AV_LOG_DEBUG("PointLight is destroyed"); 
    }
} // namespace avion::core
