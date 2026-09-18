#ifndef AVION_CORE_COMMON_EVENT_QUEUE_H
#define AVION_CORE_COMMON_EVENT_QUEUE_H 1

  #include <deque>
  #include <vector>
  #include <optional>
  #include <mutex>

  #include "AvionEngineCore/core/EventManager/EventType.hpp"

  namespace avion::core::common
  {

    class EventQueue
    {
      public:
        using EventChangedFiles = avion::core::eventmanager::detail::EventChangedFiles;

        EventQueue() = default;

        EventQueue(const EventQueue& other) = delete;
        EventQueue(EventQueue&& other) = delete;

        EventQueue& operator=(const EventQueue& other) = delete;
        EventQueue& operator=(EventQueue&& other) = delete;

        void Push(EventChangedFiles event) noexcept;
        std::optional<std::vector<EventChangedFiles>> Consume() noexcept;
        bool IsEmpty() const noexcept;


        ~EventQueue() = default;
      protected:
      private:
        std::mutex m_mutex;
        std::deque<EventChangedFiles> m_queue;
        bool m_is_empty{};

    };

  } // namespace avion::core::common

#endif