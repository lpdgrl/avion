#ifndef AVION_API_BACKEND_OPENGL_H
#define AVION_API_BACKEND_OPENGL_H

    #include "glad/glad.h"

    namespace avion::api::backend::opengl
    {
       class OpenglRenderer
       {
        public:
          OpenglRenderer() = default;

          OpenglRenderer(const OpenglRenderer&) = delete;
          OpenglRenderer(OpenglRenderer&&)      = delete;

          OpenglRenderer& operator=(const OpenglRenderer&) = delete;
          OpenglRenderer& operator=(OpenglRenderer&&)       = delete;

          ~OpenglRenderer() = default;
        protected:

        private:
       };
    } // namespace avion::api::backend::opengl

#endif