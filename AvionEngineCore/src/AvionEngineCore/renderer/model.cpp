#include "AvionEngineCore/renderer/model.hpp"
#include "AvionEngineCore/core/resource_manager.hpp"

namespace avion::gfx
{

  Model::Model(const std::string& filename, const ModelData& model_data, const Material& material)
  : m_filename(filename)
  , m_data(model_data)
  , m_transform{.position{0.f, 0.f, 0.f}, .size{1.f}, .value_rotate{0.f}, .axis = AxisRotate::NONE}
  , m_material(material)
  {

  }

  Model::Model(const std::string& filename, const ModelData& model_data, Color color)
  : m_filename(filename)
  , m_data(model_data)
  , m_transform{.position{0.f, 0.f, 0.f}, .size{1.f}, .value_rotate{0.f}, .axis = AxisRotate::NONE}
  , m_solid_color(color)
  {

  }

  Model::Model(const std::string& filename, const Model& other)
  : m_filename(filename)
  , m_data(other.m_data)
  , m_transform(other.m_transform)
  , m_material(other.m_material)
  , m_solid_color(other.m_solid_color)
  {
     AV_LOG_DEBUG("Model::Model(const std::string& filename, const Model& other): " + m_filename);
  }

  Model::Model(const Model& other)
  : m_filename(other.m_filename)
  , m_data(other.m_data)
  , m_transform(other.m_transform)
  , m_material(other.m_material)
  , m_solid_color(other.m_solid_color)
  {
    AV_LOG_DEBUG("Model::Model(const Model& other): " + m_filename);
  }

  Model::Model(Model&& other)
  : m_filename(std::move(other.m_filename))
  , m_data(std::move(other.m_data))
  , m_transform(std::move(other.m_transform))
  , m_material(std::move(m_material))
  , m_solid_color(std::move(m_solid_color))
  {

  }

  Model& Model::operator=(const Model& other)
  {
    AV_LOG_DEBUG("Model& Model::operator=(const Model& other) " + other.m_filename);

    if (this == &other)
    {
      return *this;
    }

    Model t_model(other);
    Swap(t_model);
    return *this;
  }

  Model& Model::operator=(Model&& other) noexcept
  {
    if (this == &other)
    {
      return *this;
    }

    Swap(other);
    return *this;
  }

  void Model::Swap(Model& other) noexcept
  {
    if (this == &other)
    {
      return;
    }

    // Swap Idiom (ADL lookup)
    using std::swap;
    swap(m_filename, other.m_filename);
    swap(m_data, other.m_data);
    swap(m_transform, other.m_transform);
  }

  std::string Model::GetFileName() const noexcept
  {
    return m_filename;
  }



  Transform& Model::GetTransform() noexcept
  {
    return m_transform;
  }

  const Transform& Model::GetTransform() const noexcept
  {
    return m_transform;
  }

  Model::ModelData& Model::GetModelData() noexcept 
  { 
    return m_data; 
  }

  const Model::ModelData& Model::GetModelData() const noexcept 
  { 
    return m_data; 
  }

  Model::MeshRange Model::GetMeshRange() noexcept 
  { 
    return {m_data.mesh_range.begin(), m_data.mesh_range.end()}; 
  }

  Model::Material& Model::GetMaterial() noexcept
  { 
    return m_material; 
  }

  const Model::Material& Model::GetMaterial() const noexcept 
  { 
    return m_material; 
  }

  Model::Color& Model::GetColor() noexcept 
  { 
    return m_solid_color; 
  }

  Model::Color Model::GetColor() const noexcept 
  { 
    return m_solid_color; 
  }

  void swap(Model& lhs, Model& rhs) noexcept
  {
    lhs.Swap(rhs);
  }
} // namespace avion::gfx
