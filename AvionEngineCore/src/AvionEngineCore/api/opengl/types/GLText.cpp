#include "AvionEngineCore/api/opengl/types/GLText.hpp"

namespace avion::api::backend::opengl
{
  auto GLText::Initialaztion(std::string_view font) -> void 
  {
    assert(!font.empty());
    
    if (FT_Init_FreeType(&m_ft_lib)) 
    {
      AV_LOG_ERROR("ERROR::FREETYPE Could not init FreeType Library");
      return;
    }

    if (FT_New_Face(m_ft_lib, font.data(), 0, &m_face)) 
    {
      AV_LOG_ERROR("ERROR::FREETYPE: Failed to load font");
      return;
    }

    FT_Set_Pixel_Sizes(m_face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    LoadAsciiSet();
  }

  auto GLText::LoadAsciiSet() -> void 
  {
    for (unsigned char c = 0; c < 128; c++) 
    {
      if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
      {
        AV_LOG_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
      }

      unsigned int texture{};
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(
          GL_TEXTURE_2D,
          0,
          GL_RED,
          m_face->glyph->bitmap.width,
          m_face->glyph->bitmap.rows,
          0,
          GL_RED,
          GL_UNSIGNED_BYTE,
          m_face->glyph->bitmap.buffer
      );
      
      // set texture options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // now store character for later use
      Character character
      {
          texture,
          glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
          glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
          static_cast<unsigned int>(m_face->glyph->advance.x)
      };
      m_characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // destroy FreeType once we're finished
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_ft_lib);
  }

  auto GLText::GetCharacter(GLchar c) const noexcept -> const Character&
  {
    return m_characters.at(c);
  }

  GLText::~GLText() 
  {

  }
} // namespace avion::api::backend::opengl