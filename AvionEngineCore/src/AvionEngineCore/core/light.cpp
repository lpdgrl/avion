#include "AvionEngineCore/core/light.hpp"

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
  : m_position(position)
  , m_ambient(ambient)
  , m_diffuse(diffuse)
  , m_specular(specular)
  , m_constant(constant)
  , m_linear(linear)
  , m_quadratic(quadratic)
  {

  }

  glm::vec3 PointLight::GetAmbientImpl() const {
      return m_ambient;
  }

  glm::vec3 PointLight::GetDiffuseImpl() const {
      return m_diffuse;
  }

  glm::vec3 PointLight::GetSpecularImpl() const {
      return m_specular;
  }

  void PointLight::SetAmbientImpl(glm::vec3 ambient) {
      m_ambient = ambient;
  }

  void PointLight::SetDiffuseImpl(glm::vec3 diffuse) {
      m_diffuse = diffuse;
  }

  void PointLight::SetSpecularImpl(glm::vec3 specular) {
      m_specular = specular;
  }

  void PointLight::SetParamsImpl() {

  }

  glm::vec3& PointLight::GetGeometryImpl() {
      return m_position;
  }

  void PointLight::SetPosition(glm::vec3 position) noexcept {
      m_position = position;
  }

  glm::vec3 PointLight::GetPosition() const noexcept {
      return m_position;
  }
  
  void PointLight::SetConstant(float constant) noexcept {
      m_constant = constant;
  }

  void PointLight::SetLinear(float linear) noexcept {
      m_linear = linear;
  }

  void PointLight::SetQuadratic(float quadratic) noexcept {
      m_quadratic = quadratic;
  }

  float PointLight::GetConstant() const noexcept {
      return m_constant;
  }
  
  float PointLight::GetLinear() const noexcept {
      return m_linear;
  }

  float PointLight::GetQuadratic() const noexcept {
      return m_quadratic;
  }

  PointLight::~PointLight() 
  {
      AV_LOG_DEBUG("PointLight is destroyed"); 
  }

  SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse,
          glm::vec3 specular, float constant, float linear, float quadratic, float cutoff, float outer_cutoff)
  : PointLight(position, ambient, diffuse, specular, constant, linear, quadratic)
  , m_direction(direction)
  , m_cutoff(cutoff)
  , m_outer_cutoff(outer_cutoff)
  {

  }

  void SpotLight::SetDirection(glm::vec3 direction) noexcept {
    m_direction = direction;
  }

  glm::vec3 SpotLight::GetDirection() const noexcept 
  {
    return m_direction;
  }

  void SpotLight::SetCutOff(float cutoff) noexcept
  {
    m_cutoff = cutoff;
  }

  float SpotLight::GetCutOff() const noexcept {
      return m_cutoff;
  }

  void SpotLight::SetOuterCutOff(float outer_cutoff) noexcept
  {
    m_outer_cutoff = outer_cutoff;
  }

  float SpotLight::GetOuterCutOff() const noexcept
  {
    return m_outer_cutoff;
  }

  SpotLight::~SpotLight() 
  {
      AV_LOG_DEBUG("SpotLight is destroyed"); 
  }

} // namespace avion::core

