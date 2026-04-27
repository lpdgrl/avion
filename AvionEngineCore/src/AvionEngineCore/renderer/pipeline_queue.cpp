#include "AvionEngineCore/renderer/pipeline_queue.hpp"
#include "AvionEngineCore/renderer/mesh.hpp"

namespace avion::gfx
{
  void PipelineQueue::Enqueue(std::vector<Mesh>& mesh_vec) noexcept 
  {
    AV_LOG_DEBUG("PipelineQueue::Enqueue: count meshes");
    AV_LOG_DEBUG(std::to_string(mesh_vec.size()));
    for (auto& mesh : mesh_vec)
    {
       m_queue_meshs.push_back(&mesh);
    }
  }

  Mesh* PipelineQueue::Dequeue() noexcept 
  {
    if (m_queue_meshs.empty())
    {
      AV_LOG_ERROR("PipelineQueue::Dequeue: queue is empty.");
      return nullptr;
    }
    
    Mesh *mesh = m_queue_meshs.front();
    m_queue_meshs.pop_front();
    return mesh;
  }

  bool PipelineQueue::IsEmpty() const noexcept
  {
    return m_queue_meshs.empty();
  }
} // namespace avion::gfx