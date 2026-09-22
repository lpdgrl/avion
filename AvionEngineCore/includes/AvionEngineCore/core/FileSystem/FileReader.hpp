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
      static auto WriteFile(const std::string& name, const std::string& data, const Path& path = m_path) -> bool;
    private:
    // TODO: It's temporary and for testing WriteFile and ReadFile
      static constexpr const char* m_path = "/home/lpdgrl/Project/code/avion/assets/scenes";
  };
} // namespace avion::core::filesystem

#endif