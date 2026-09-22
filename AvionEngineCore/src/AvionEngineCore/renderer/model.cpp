#include "AvionEngineCore/renderer/model.hpp"
#include "AvionEngineCore/core/resource_manager.hpp"

namespace avion::gfx
{

  Model::Model(std::string_view filename, CpuModelData& cpu_model_data, const Material& material, bool has_animation,  std::string_view extension)
  : m_filename(filename)
  , m_file_extension(extension)
  , m_cpu_model_data(cpu_model_data)
  , m_transform{.position{0.f, 0.f, 0.f}, .size{0.005f}, .value_rotate{0.f}, .axis = AxisRotate::NONE}
  , m_material(material)
  , m_animator(m_cpu_model_data)
  , m_has_animation(has_animation)
  {

  }

  Model::Model(std::string_view filename, const Model& other)
  : m_filename(filename)
  , m_file_extension(other.m_file_extension)
  , m_cpu_model_data(other.m_cpu_model_data)
  , m_transform(other.m_transform)
  , m_material(other.m_material)
  , m_animator(m_cpu_model_data)
  , m_has_animation(other.m_has_animation)
  {
     AV_LOG_DEBUG("Model::Model(const std::string& filename, const Model& other): " + m_filename);
  }

  Model::Model(const Model& other)
  : m_filename(other.m_filename)
  , m_file_extension(other.m_file_extension)
  , m_cpu_model_data(other.m_cpu_model_data)
  , m_transform(other.m_transform)
  , m_material(other.m_material)
  , m_animator(other.m_animator)
  , m_has_animation(other.m_has_animation)
  {
    AV_LOG_DEBUG("Model::Model(const Model& other): " + m_filename);
  }

  Model::Model(Model&& other)
  : m_filename(std::move(other.m_filename))
  , m_file_extension(std::move(other.m_file_extension))
  , m_cpu_model_data(std::move(other.m_cpu_model_data))
  , m_transform(std::move(other.m_transform))
  , m_material(std::move(other.m_material))
  , m_animator(std::move(other.m_animator))
  , m_has_animation(other.m_has_animation)
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
    swap(m_file_extension, other.m_file_extension);
    swap(m_cpu_model_data, other.m_cpu_model_data);
    swap(m_transform, other.m_transform);
    swap(m_material, other.m_material);
    swap(m_has_animation, other.m_has_animation);
  }

  std::string Model::GetFileName() const noexcept
  {
    return m_filename;
  }

  auto Model::GetFileExtension() const noexcept -> std::string
  {
    return m_file_extension;
  }

  Transform& Model::GetTransform() noexcept
  {
    return m_transform;
  }

  const Transform& Model::GetTransform() const noexcept
  {
    return m_transform;
  }

  Model::CpuModelData& Model::GetCpuModelData() noexcept 
  { 
    return m_cpu_model_data; 
  }

  const Model::CpuModelData& Model::GetCpuModelData() const noexcept 
  { 
    return m_cpu_model_data; 
  }

  Model::MeshRange Model::GetMeshRange() noexcept 
  { 
    return {m_cpu_model_data.mesh_range.begin(), m_cpu_model_data.mesh_range.end()}; 
  }

  Model::Material& Model::GetMaterial() noexcept
  { 
    return m_material; 
  }

  const Model::Material& Model::GetMaterial() const noexcept 
  { 
    return m_material; 
  }

   auto Model::HasAnimation() const noexcept -> bool
   {
    return m_has_animation;
   }

   auto Model::GetAnimator() noexcept -> Animator&
   {
    return m_animator;
   }

  void swap(Model& lhs, Model& rhs) noexcept
  {
    lhs.Swap(rhs);
  }
} // namespace avion::gfx
