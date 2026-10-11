#ifndef AVION_API_BACKEND_OPENGL_TYPES_GL_TEXT_H
#define AVION_API_BACKEND_OPENGL_TYPES_GL_TEXT_H 1

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>
#include <string>
#include <string_view>

#include "AvionEngineCore/macro.h"

#include <glm/glm.hpp>
#include "glad/glad.h"

namespace avion::api::backend::opengl {

  struct Character {
      unsigned int texture_id;
      glm::ivec2 size;        // Size of glyph
      glm::ivec2 bearing;     // Offset from baseline to left/top of glyph
      unsigned int advance;   // Horizontal offset to advance to next glyph
  };

  class GLText {
  public:

    GLText() = default;
    ~GLText();

    auto Initialaztion(std::string_view font) -> void;
    auto GetCharacter(GLchar c) const noexcept -> const Character&;
      
  private:
    auto LoadAsciiSet() -> void;

  private:
    std::unordered_map<GLchar, Character> m_characters;
    FT_Library m_ft_lib;
    FT_Face m_face;
  };

} // avion::api::backend::opengl

#endif