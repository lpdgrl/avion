#include "AvionEngineCore/core/ResourceManager/FileWatcher.hpp"

namespace avion::core::resman::filewatcher
{

  FileWatcher::FileWatcher(EventQueue& event_queue, const std::vector<FsPath>& paths)
  : m_queue(event_queue)
  , m_worker(std::thread(&FileWatcher::Running, &(*this)))
  {
    PathToFileStamp(paths);
  }

  FileWatcher::~FileWatcher() 
  {
    m_running = false;
    if (m_worker.joinable())
    {
      m_worker.join();
    }
  }

  void FileWatcher::Running() noexcept
  {
    using namespace std::literals::chrono_literals;
    m_running = true;
    while (m_running)
    {
      if (!m_paths.empty())
      {
        bool changed_files = CheckFiles();
        if (changed_files)
        {
          while(!m_indices_queue.empty())
          {
            auto& path = m_indices_queue.front();
            AV_LOG_DEBUG(std::format("File {} is changed is time {}", path->path_to_file, path->file_time));
            m_queue.Push({path->path_to_file});
            m_indices_queue.pop_front();
          }
        }
        std::this_thread::sleep_for(5000ms);
      }
    }
  }


  bool FileWatcher::CheckFiles() noexcept
  {
    bool result{};
    for (auto& file_timestamp : m_paths)
    {
      std::filesystem::file_time_type ftime = std::filesystem::last_write_time(file_timestamp.path_to_file);
      if (ftime != file_timestamp.file_time)
      {
        result = true;
        file_timestamp.file_time = ftime;
        m_indices_queue.push_back(&file_timestamp);
      }
    }
    return result;
  }

  void FileWatcher::PathToFileStamp(const std::vector<FsPath>& paths) noexcept
  {
    if (paths.empty())
    {
      return;
    }

    m_paths.reserve(paths.size());

    // Интересно, возможно ли здесь применить ренджи?
    for (const auto& path : paths)
    {
      m_paths.emplace_back(path, std::filesystem::last_write_time(path));
    }
  }
} // avion::core::resman::filewatcher
