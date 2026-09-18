#include "AvionEngineCore/core/Common/EventQueue.hpp"
namespace avion::core::common
{

  void EventQueue::Push(EventChangedFiles event) noexcept
  {
    std::scoped_lock scope_lock(m_mutex);
    m_queue.push_back(event);
    m_is_empty = false;
  }

  std::optional<std::vector<EventQueue::EventChangedFiles>> EventQueue::Consume() noexcept
  {
    std::optional<std::vector<EventChangedFiles>> result;
    std::vector<EventChangedFiles> v_res;
    v_res.reserve(m_queue.size());

    std::scoped_lock scope_lock(m_mutex);
    if (m_queue.empty())
    {
      return result;
    }

    while (!m_queue.empty())
    {
      v_res.emplace_back(m_queue.front());
      m_queue.pop_front();
    }
    result = std::move(v_res);
    m_is_empty = true;
    return result;
  }

  bool EventQueue::IsEmpty() const noexcept
  {
    return m_is_empty;
  }

} // namespace avion::common
