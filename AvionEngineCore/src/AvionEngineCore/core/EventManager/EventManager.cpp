#include "AvionEngineCore/core/EventManager/EventManager.hpp"

namespace avion::core::eventmanager
{

  void EventManager::Notify(EventType type, int action, float value) noexcept
  {
    auto it = m_listeners.find(type);
    assert(it == m_listeners.end() && "EventManager::Notify type isn't register");

    switch(type)
    {
      case EventType::CameraEvent:
      {
        auto& observer = *(static_cast<detail::CameraListener*>(it->second.get()));
      }
    }

  }

  void EventManager::Notify(EventType type) noexcept
  {

  }

  void EventManager::Subscribe(EventType type, ListenerType listener, void *observer) noexcept
  {
     auto it = m_listeners.find(type);
     assert(it != m_listeners.end() && "EventManager::Subscribe: listener already subscribe");

     m_listeners.emplace(type, MakeEventListener(listener, observer));
  }

  void EventManager::Unsubscribe(EventType type) noexcept
  {
    auto it = m_listeners.find(type);
    assert(it == m_listeners.end() && "EventManager::Unsubscribe listener isn't subscribe");

    m_listeners.erase(it);
  }

  EventManager::EventListenerPtr EventManager::MakeEventListener(ListenerType type, void *observer) const noexcept
  {
    switch(type)
    {
      case ListenerType::CameraListener:
      {
        return std::make_unique<detail::CameraListener>(observer);
      }
    }
  }
} // namespace avion::core::eventmanager