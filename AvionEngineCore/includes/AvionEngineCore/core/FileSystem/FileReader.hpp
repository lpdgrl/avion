#ifndef AVION_CORE_FILE_SYSTEM_FILE_READER_H
#define AVION_CORE_FILE_SYSTEM_FILE_READER_H 1

#include <string>
#include <fstream>
#include <filesystem>

#include "AvionEngineCore/macro.h"

namespace avion::core::filesystem
{
  class FileReader
  {
    public:
      using Path = std::filesystem::path;
      static auto ReadFile(const Path& path) -> std::string;
  };
} // namespace avion::core::filesystem

#endif