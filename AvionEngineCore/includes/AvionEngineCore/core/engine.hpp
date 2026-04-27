#ifndef AVION_CORE_ENGINE_H 
#define AVION_CORE_ENGINE_H

  #include "window.hpp"
  #include "resource_manager.hpp"

  namespace avion::core
  {
    class Engine 
    {
      public:
        Engine() = delete;
        Engine(const char* name_window, int width, int height);

        void Init();
        void Update();
        void Shutdown();

      private:
        std::unique_ptr<Window> m_window;
        std::string             m_version_engine = "0.0.1";
    };
  } // namespace avion::core

#endif