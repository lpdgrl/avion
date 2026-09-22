#include "AvionEngineCore/core/ModelManager/Detail/Utils.hpp"

namespace avion::core::modelmanager::detail
{
  auto GetFileName(std::string_view full_filename) -> std::string
  {
    std::string filename;
    if (full_filename.empty())
    {
      AV_LOG_ERROR(std::format("GetFileName(std::string_view full_filename): filename {} is empty", full_filename));
      return filename;
    }
    
    char delim = '.';
    if (const auto result = ParseString(full_filename, delim); result.has_value())
    {
      filename = std::move(result.value());
    } else if (result.error() == ParseStringError::NotFindDelim)
    {
      AV_LOG_ERROR(std::format("GetFileName(std::string_view full_filename): specific delim  isn't finded = {}", delim));
    }
    return filename;
  } 

  auto ParseString(std::string_view str, char delim) -> std::expected<std::string, ParseStringError>
  {
    auto pos = str.find(delim);
    if (pos == std::string::npos)
    {
      return std::unexpected(ParseStringError::NotFindDelim);
    }
    auto res = str.substr(0, pos);

    std::string parse_result(res.begin(), res.end());
    return parse_result;
  }

} // namespace avion::core::modelmanager::detail