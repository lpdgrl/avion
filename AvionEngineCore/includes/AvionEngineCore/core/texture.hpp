
#ifndef AVION_CORE_TEXTURE_H
#define AVION_CORE_TEXTURE_H
    
    #include "../../glad/glad.h"
    
    #include "../../stb/stb_image.h"

    #include "../macro.h"

    #include <string>
    #include <cstdint>  
    #include <utility>

    namespace avion::core {

        class Texture {
        public:
            Texture() = default;
            
            Texture(const Texture& other) = delete;
            Texture& operator=(const Texture& other) = delete;

            Texture(Texture&& other);
            Texture& operator=(Texture&& other);

            explicit Texture(const std::string& path_texture);
            explicit Texture(const char* path_texture);
            explicit Texture(std::string_view path_texture);

            bool LoadTexture();
            bool FreeTexture();
            void SwitchPathToTexture(const std::string& path_texture);

            std::uint32_t&        GetId()             noexcept;
            
            std::uint16_t         GetWidth()          const noexcept;
            std::uint16_t         GetHeight()         const noexcept;
            GLenum                GetColorChannels()  const noexcept;
            const unsigned char*  GetBuffer()         const noexcept;
            std::string           GetPath()           const noexcept;

            bool IsUploaded() const noexcept;
            
            ~Texture(); 

        private:
            std::string m_path;
            std::uint32_t m_index{};
            std::uint32_t m_num_color_channels{};
            std::uint16_t m_width{};
            std::uint16_t m_height{};
            std::uint8_t* m_buffer = nullptr;
            bool m_is_uploaded = false;
        };
    } // namespace avion:;core
     
#endif // AVION_CORE_TEXTURE_H 
