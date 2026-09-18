#ifndef AVION_CORE_RESOURCE_MANAGER_FILE_WATCHER_H
#define AVION_CORE_RESOURCE_MANAGER_FILE_WATCHER_H 1
  
  #include <vector>
  #include <string>
  #include <filesystem>
  #include <chrono>
  #include <thread>

  #include "AvionEngineCore/macro.h"
  #include "AvionEngineCore/core/Common/EventQueue.hpp"

  namespace avion::core::resman::filewatcher
  {
    struct FileTimeStamp
    {
      std::filesystem::path path_to_file;
      std::filesystem::file_time_type file_time{};
    };
    class FileWatcher
    {
      public:
        using EventQueue = avion::core::common::EventQueue;
        using EventChangedFiles = avion::core::eventmanager::detail::EventChangedFiles;
        using FsPath = std::filesystem::path;

        FileWatcher() = delete;
        FileWatcher(EventQueue& event_queue, const std::vector<FsPath>& paths);

        FileWatcher(const FileWatcher& other) = delete;
        FileWatcher(FileWatcher&& other) = delete;

        FileWatcher& operator=(const FileWatcher& other) = delete;
        FileWatcher& operator=(FileWatcher&& other) = delete;

        ~FileWatcher();

        void Running() noexcept;

      private:
        bool CheckFiles() noexcept;
        void PathToFileStamp(const std::vector<FsPath>& paths) noexcept;

      protected:
      private:
        EventQueue& m_queue;
        std::deque<FileTimeStamp*> m_indices_queue;
        std::vector<FileTimeStamp> m_paths;
        bool m_running{};
        std::thread m_worker;
    };
  } // avion::core::resman:filewatcher

#endif