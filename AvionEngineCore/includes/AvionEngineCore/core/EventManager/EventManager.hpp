#ifndef AVION_CORE_EVENT_MANAGER_H
#define AVION_CORE_EVENT_MANAGER_H 1
  
  #include "AvionEngineCore/core/EventManager/EventListener.hpp"
  #include "AvionEngineCore/core/EventManager/EventType.hpp"

  #include <unordered_map>
  #include <memory>
  #include <cassert>

  namespace avion::core::eventmanager
  {
    class EventManager
    {
      using EventListener = detail::EventListener;
      using EventType = detail::EventType;
      using EventListenerPtr = std::unique_ptr<EventListener>;

      using Listeners = std::unordered_map<EventType, EventListenerPtr>;
      using ListenerType = detail::ListenerType;

      public:
        EventManager() = default;
        ~EventManager() = default;
        
        void Subscribe(EventType type, ListenerType listener_type, void *observer) noexcept;
        void Unsubscribe(EventType type) noexcept;

        void Notify(EventType type) noexcept;
        void Notify(EventType type, int action, float value) noexcept;
      protected:

      private:
        EventListenerPtr MakeEventListener(ListenerType type, void *observer) const noexcept; 

        Listeners m_listeners;
        
    };
  } // namespace avion::core::eventmanager

#endif