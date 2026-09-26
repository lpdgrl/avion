#include "AvionEngineCore/core/FileSystem/FileReader.hpp"

namespace avion::core::filesystem
{
  auto FileReader::ReadFile(const Path& path) -> std::string
  {
    std::string code;
    std::ifstream file;
    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
      // open files
      file.open(path);
      std::stringstream string_stream_file;

      //read file's buffer contents into stream
      string_stream_file << file.rdbuf();

      // close file handlers
      file.close();

      // convert stream into string
      code = string_stream_file.str();
    }
    catch(std::ifstream::failure& e) 
    {
      std::string exception(e.what());
      AV_LOG_ERROR(std::format("FileReader::ReadFile(const Path& path): File no succesfully read {} {}", path, exception));
    }
      return code;
  }

  auto FileReader::WriteFile(const std::string& name, const std::string& data, const Path& path) -> bool
  {
    std::string code;
    std::ofstream file;
    file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    try 
    {
      file.open(path / name, std::ios::ate);
      if (!file.is_open())
      {
        AV_LOG_ERROR(std::format("FileReader::WriteFile: File {} isn't open", name));
        return false;
      }
      file.write(data.data(), data.size());
      file.close();
    }
    catch(std::ofstream::failure& e) 
    {
      std::string exception(e.what());
      AV_LOG_ERROR(std::format("FileReader::WriteFile: File no succesfully read {} {}", path, exception));
      return false;
    }
    return true;
  }
  
} // namespace avion::core::filesystem