#ifndef AVION_CORE_MODEL_MANAGER_DETAIL_UTILS_H
#define AVION_CORE_MODEL_MANAGER_DETAIL_UTILS_H 1

#include <string>
#include <string_view>
#include <expected>

#include "AvionEngineCore/macro.h"

namespace avion::core::modelmanager::detail
{
  enum class ParseStringError
  {
    NotFindDelim = 1,
  };

  auto GetFileName(std::string_view full_filename) -> std::string;
  auto ParseString(std::string_view str, char delim) -> std::expected<std::string, ParseStringError>;
} // namespace avion::core::modelmanager::detail

#endif