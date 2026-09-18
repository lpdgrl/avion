#ifndef AVION_CORE_EVENT_TYPE_H 
#define AVION_CORE_EVENT_TYPE 1

  #include <cstdint>
  #include <filesystem>
  namespace avion::core::eventmanager::detail
  {
    struct EventChangedFiles 
    {
      std::filesystem::path path;
    };
    
    enum class EventTypeFileSystem : std::uint32_t
    {
      Modified = 0,
    };

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