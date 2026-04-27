#ifndef AVION_GFX_PIPELINE_QUEUE_HPP
#define AVION_GFX_PIPELINE_QUEUE_HPP

  #include <deque>
  #include <vector>

  namespace avion::gfx 
  {
    class Mesh;

    class PipelineQueue
    {
      public:
      PipelineQueue()  = default;
      ~PipelineQueue() = default;
      
      void Enqueue(std::vector<Mesh>& mesh) noexcept;
      Mesh* Dequeue() noexcept;

      bool IsEmpty() const noexcept;

      private:
      std::deque<Mesh*> m_queue_meshs;
    };
  } // namespace avion::gfx
  
#endif