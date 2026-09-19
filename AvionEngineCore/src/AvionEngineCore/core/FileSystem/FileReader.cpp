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
      AV_LOG_ERROR(std::format("FileReader::ReadFile(const Path& path): Shader file no succesfully read {} {}", path, exception));
    }
      return code;
  }
  
} // namespace avion::core::filesystem