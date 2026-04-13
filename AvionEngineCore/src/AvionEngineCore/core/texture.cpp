#include "../includes/AvionEngineCore/core/texture.hpp"

namespace avion::core {
    
    Texture::Texture(const std::string& path_texture)
    : m_index{}
    , m_path(path_texture)
    {

    }

    Texture::Texture(const char* path_texture)
    : m_index{}
    , m_path(path_texture)
    {

    }

    Texture::Texture(std::string_view path_texture)
    : m_index{}
    , m_path(path_texture)
    {

    }

    Texture::Texture(Texture&& other)
    : m_index(std::exchange(other.m_index, 0))
    , m_path(std::move(other.m_path))
    , m_buffer(other.m_buffer)
    {
        other.m_index = 0;
        other.m_buffer = nullptr;
    }

    Texture& Texture::operator=(Texture&& other)
    {
        if (this != &other) {
            m_index = std::exchange(other.m_index, 0);
            m_path = std::move(other.m_path);
            m_buffer = std::exchange(other.m_buffer, nullptr);
        }

        return *this;
    }

    bool Texture::LoadTexture()
    {
        if (IsUploaded()) {
            AV_LOG_ERROR("Texture::LoadTexture: texture is already uploaded" + m_path);
            return false;
        }

        int width = 0;
        int height = 0;
        int num_color_channels = 0;

        m_buffer = stbi_load(m_path.c_str(), &width, &height, &num_color_channels, 0);
          //if (stbi_failure_reason()) {
        if (!m_buffer) {
            AV_LOG_ERROR("Texture::LoadTexture: Texture failed to load at path: " + m_path);

            auto er_stbi = stbi_failure_reason();
            
            std::string error; 
            error += "stbi failure reason ";
            error += er_stbi;

            AV_LOG_ERROR(error); 

            stbi_image_free(m_buffer);
            m_buffer = nullptr;

            return false;
        }

        m_width = width;
        m_height = height;
        m_num_color_channels = num_color_channels;

        m_is_uploaded = true; 

        return m_is_uploaded;
    }
    
    // That's too promising a name
    bool Texture::FreeTexture() {
        if (m_buffer) 
        {
            m_width = 0;
            m_height = 0;
            m_num_color_channels = 0;
            m_path.clear();
            m_is_uploaded = false;
            
            stbi_image_free(m_buffer);
            return true;
        }
        return false;
    }

    bool Texture::IsUploaded() const noexcept {
        return m_is_uploaded;
    }

    std::uint32_t& Texture::GetId() noexcept {
        return m_index;
    }

    unsigned char* Texture::GetBuffer() noexcept {
        return m_buffer; 
    }

    std::uint16_t Texture::GetWidth() const noexcept
    {
      return m_width;
    }
    
    std::uint16_t Texture::GetHeight() const noexcept
    {
      return m_height;
    }

    GLenum Texture::GetColorChannels() const noexcept
    {
        GLenum format;
        if (m_num_color_channels == 1) {
            format = GL_RED;
        } else if (m_num_color_channels == 3) {
            format = GL_RGB;
        } else if (m_num_color_channels == 4) {
            format = GL_RGBA;
        }

        return format;
    }

    void Texture::SwitchPathToTexture(const std::string& path_texture) {
        AV_LOG_TODO("Texture::SwitchTextureFile: is nothing done");
    }


    Texture::~Texture() {
        if (m_buffer) {
            stbi_image_free(m_buffer);
            AV_LOG_INFO("Texture::~Texture: texture's deleted. Index:" + std::to_string(m_index) + " Path: " + m_path);
        }
    }

    std::string Texture::GetPath() const noexcept {
        if (m_path.empty()) {
            AV_LOG_ERROR("Texture::GetPath: path haven't specified to texture"); 
            return {};
        }
        return m_path;
    }

} // namespace avion::core
