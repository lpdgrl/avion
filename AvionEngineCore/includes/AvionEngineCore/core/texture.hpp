
#ifndef AVION_CORE_TEXTURE_H
#define AVION_CORE_TEXTURE_H
    
    #include "../../glad/glad.h"
    
    #include "../../stb/stb_image.h"

    #include "../macro.h"

    #include <string>
    #include <cstdint>  

    namespace avion::core {

        class Texture {
        public:
            Texture() = delete;
            
            Texture(const Texture& other) = delete;
            Texture& operator=(const Texture& other) = delete;

            Texture(Texture&& other);
            Texture& operator=(Texture&& other);

            explicit Texture(const std::string& path_to_tex);
            explicit Texture(const char* path_to_tex);
            
            bool LoadTexture();
            
            // TODO: That's too promising a name
            bool FreeTexture();
             
            std::uint32_t GetId() const noexcept;
            const unsigned char* GetBuffer() const noexcept;

            void SwitchTextureFile(const std::string& path_to_tex);
            bool IsLoaded() const noexcept;
            std::string GetPath() const noexcept;

            ~Texture(); 

        private:
            bool is_loaded_ = false;
            std::uint32_t id_tex_;
            std::string path_to_tex_;
            unsigned char* buffer_ = nullptr;
        };
    } // namespace avion:;core
     
#endif // AVION_CORE_TEXTURE_H 
