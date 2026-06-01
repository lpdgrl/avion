#ifndef AVION_API_BACKEND_OPENGL_TYPES_H
#define AVION_API_BACKEND_OPENGL_TYPES_H

  #include <cstddef>
  #include <cstdint>

  #include "glad/glad.h"

  namespace avion::api::backend::opengl::detail
  {
    enum class CompareFunc
    {
      fnNever         = GL_NEVER,
      fnLess          = GL_LESS,
      fnLessEqual     = GL_LEQUAL,
      fnGreater       = GL_GREATER,
      fnGreaterEqual  = GL_GEQUAL,
      fnEqual         = GL_EQUAL,
      fnNotEqual      = GL_NOTEQUAL,
      fnAlways        = GL_ALWAYS,
    };

    enum class StencilAction
    {
      fnKeep          = GL_KEEP,
      fnZero          = GL_ZERO,
      fnReplace       = GL_REPLACE,
      fnIncrement     = GL_INCR,
      fnIncrementWrap = GL_INCR_WRAP,
      fnDecrement     = GL_DECR,
      fnDecrementWrap = GL_DECR_WRAP,
      fnInvert        = GL_INVERT,
    };

    enum class BlendFunc
    {
      fnZero                      = GL_ZERO,
      fnOne                       = GL_ONE,
      fnSourceColor               = GL_SRC_COLOR,
      fnOneMinusSourceColor       = GL_ONE_MINUS_SRC_COLOR,
      fnDestinationColor          = GL_DST_COLOR,
      fnOneMinusDestinationColor  = GL_ONE_MINUS_DST_COLOR,
      fnSourceAlpha               = GL_SRC_ALPHA,
      fnOneMinusSourceAlpha       = GL_ONE_MINUS_SRC_ALPHA,
      fnDestinationAlpha          = GL_DST_ALPHA,
      fnOneMinusDestinationAlpha  = GL_ONE_MINUS_DST_ALPHA,
      fnConstantColor             = GL_CONSTANT_COLOR,
      fnOneMinusConstantColor     = GL_ONE_MINUS_CONSTANT_COLOR,
      fnConstantAlpha             = GL_CONSTANT_ALPHA,
      fnOneMinusConstantAlpha     = GL_ONE_MINUS_CONSTANT_ALPHA,
    };

    enum class BlendEquation
    {
      fnAdd             = GL_FUNC_ADD,
      fnSubtract        = GL_FUNC_SUBTRACT,
      fnReverseSubtract = GL_FUNC_REVERSE_SUBTRACT,
      fnMin             = GL_MIN,
      fnMax             = GL_MAX,
    };

    enum class DrawPrimitive
    {
      kPoints        = GL_POINTS,
      kLines         = GL_LINES,
      kLineStrip     = GL_LINE_STRIP,
      kTriangles     = GL_TRIANGLES,
      kTriangleStrip = GL_TRIANGLE_STRIP,
      kTriangleFan   = GL_TRIANGLE_FAN,
    };

    enum class IndexType
    {
      kUnsignedByte  = GL_UNSIGNED_BYTE,
      kUnsignedShort = GL_UNSIGNED_SHORT,
      kUnsignedInt   = GL_UNSIGNED_INT,
    };

    enum class VertexAttributeType
    {
      kByte          = GL_BYTE,
      kUnsignedByte  = GL_UNSIGNED_BYTE,
      kShort         = GL_SHORT,
      kUnsignedShort = GL_UNSIGNED_SHORT,
      kInt           = GL_INT,
      kUnsignedInt   = GL_UNSIGNED_INT,
      kFloat         = GL_FLOAT,
      kDouble        = GL_DOUBLE,
    };

    struct VertexAttribute
    {
      std::uint32_t index{};
      std::int32_t  size{};
      VertexAttributeType type = VertexAttributeType::kFloat;
      bool normalized{};
      std::size_t offset{};
    };
  } // namespace avion::api::backend::opengl::detail

#endif
