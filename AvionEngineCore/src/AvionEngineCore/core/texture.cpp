#include "../includes/AvionEngineCore/core/texture.hpp"

namespace avion::core {
    
    Texture::Texture(const std::string& path_to_tex)
    : id_tex_{}
    , path_to_tex_(path_to_tex)
    {

    }

    Texture::Texture(const char* path_to_tex)
    : id_tex_{}
    , path_to_tex_(path_to_tex)
    {

    }

    Texture::Texture(Texture&& other)
    : id_tex_(other.id_tex_)
    , path_to_tex_(std::move(other.path_to_tex_))
    , buffer_(other.buffer_)
    {
        other.id_tex_ = 0;
        other.buffer_ = nullptr;
    }

    Texture& Texture::operator=(Texture&& other)
    {
        if (this != &other) {
            id_tex_ = other.id_tex_;
            path_to_tex_ = std::move(other.path_to_tex_);
            buffer_ = other.buffer_;

            other.id_tex_ = 0;
            other.buffer_ = nullptr;
        }

        return *this;
    }

    bool Texture::LoadTexture()
    {
        if (IsLoaded()) {
            AV_LOG_ERROR("Texture already loaded" + path_to_tex_);
            return false;
        }

        int width = 0;
        int height = 0;
        int num_color_channels = 0;

        buffer_ = stbi_load(path_to_tex_.c_str(), &width, &height, &num_color_channels, 0);
        if (stbi_failure_reason()) {
            AV_LOG_ERROR("Texture failed to load at path: " + path_to_tex_);

            auto er_stbi = stbi_failure_reason();
            
            std::string error; 
            error += "stbi failure reason ";
            error += er_stbi;

            AV_LOG_ERROR(error); 

            stbi_image_free(buffer_);
            buffer_ = nullptr;

            return false;
        }
        
        GLenum format;
        if (num_color_channels == 1) {
            format = GL_RED;
        } else if (num_color_channels == 3) {
            format = GL_RGB;
        } else if (num_color_channels == 4) {
            format = GL_RGBA;
        } else {
            format = GL_RED;
        }

        glGenTextures(1, &id_tex_);
        glBindTexture(GL_TEXTURE_2D, id_tex_);
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buffer_);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
        stbi_image_free(buffer_);
        buffer_ = nullptr;

        is_loaded_ = true; 

        return true;
    }
    
    // That's too promising a name
    bool Texture::FreeTexture() {
        return false;
    }

    bool Texture::IsLoaded() const noexcept {
        return is_loaded_;
    }

    std::uint32_t Texture::GetId() const noexcept {
        return id_tex_;
    }

    const unsigned char* Texture::GetBuffer() const noexcept {
        return buffer_; 
    }

    void Texture::SwitchTextureFile(const std::string& path_to_tex) {
        path_to_tex_ = path_to_tex;
    }

    Texture::~Texture() {
        if (buffer_) {
            stbi_image_free(buffer_);
        }
    }

    std::string Texture::GetPath() const noexcept {
        if (path_to_tex_.empty()) {
            AV_LOG_ERROR("Path to texture doesn't specified"); 
            return {};
        }
        return path_to_tex_;
    }

} // namespace avion::core
