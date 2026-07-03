#ifndef AVION_CORE_EVENT_TYPE_H 
#define AVION_CORE_EVENT_TYPE 1

  #include <cstdint>

  namespace avion::core::eventmanager::detail
  {
    enum class EventType : std::uint32_t
    {
      CameraEvent = 0,

    };

    enum class ListenerType 
    {
      CameraListener = 0,
      
    };
  } // namespace avion::core::eventmanager::detail

#endif