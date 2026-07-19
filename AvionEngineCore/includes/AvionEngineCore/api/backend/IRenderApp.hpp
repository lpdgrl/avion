#ifndef AVION_CORE_IRENDER_APP_H
#define AVION_CORE_IRENDER_APP_H 1

  namespace avion::core::common
  {
    class IRenderApp 
    {
      public:
        IRenderApp() = default;
        virtual ~IRenderApp() = default;

        virtual void Render() = 0;
    };
  } // namespace avion::core::common


#endif 