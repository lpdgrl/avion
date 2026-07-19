#ifndef AVION_CORE_EVENT_LISTENER_H
#define AVION_CORE_EVENT_LISTENER_H 1

  namespace avion::gfx
  {
    class Camera;
  }

  namespace avion::core::eventmanager::detail
  {
    class EventListener
    {
      public:
        EventListener() = default;
        
        EventListener(const EventListener&) = delete("Copy constructor feat disable");
        EventListener(EventListener&&) = delete("Move constructor feat disable");

        EventListener& operator=(const EventListener&) = delete("Copy assignment feat disable");
        EventListener& operator=(EventListener&&) = delete("Move assignment feat disable");

        virtual ~EventListener() = default;

        // virtual void Update() = 0;
    };

    class CameraListener : public EventListener
    {
      public:
        CameraListener(void* observer);
        
        ~CameraListener() = default;

        void Update(int action, float value);
        void Update(double xoffset, double yoffset);
      private:
        gfx::Camera& m_camera;
    };
  } // namespace avion::core::eventmanager::detail

#endif