#pragma once

#include <iostream>
#include <chrono>
#include <format>
#include <string>
#include <source_location>

namespace avion::logger {
  enum class LogLevel {
      kInformation = 0,
      kDebug = 1,
      kWarning = 2,
      kError = 3,
      kCritical = 4,
      kToDo = 5,
  };
  
  class Logger {
    private:
      static constexpr std::string RESET_CLR{"\033[0m"};
      static constexpr std::string RED_CLR{"\033[31m"};
      static constexpr std::string GREEN_CLR{"\033[32m"};
      static constexpr std::string YELLOW_CLR{"\033[33m"};
      static constexpr std::string BLUE_CLR{"\033[34m"};
      static constexpr std::string MAGENTA_CLR{"\033[35m"};

    public:
      Logger()  = delete;
      ~Logger() = delete;
      
      static constexpr void Log(std::source_location src_loc, 
        const std::string& msg
      ) noexcept
      {
        LogDebug(src_loc, msg);
      }

      static constexpr void Log(LogLevel level, const std::string& arg) noexcept 
      {
          switch (level) 
          {
              case LogLevel::kInformation:
                  LogInfo(arg);
                  break;
              case LogLevel::kDebug:
                  LogDebug(arg);
                  break;
              case LogLevel::kError:
                  LogError(arg);
                  break;
              case LogLevel::kWarning:
                  LogWarning(arg);
                  break;
              case LogLevel::kCritical:
                  LogCritical(arg);
                  break;
              case LogLevel::kToDo:
                  LogToDo(arg);
          }
      }

      static constexpr void LogInfo(const std::string& arg) noexcept 
      {
        std::cout << FormatMessage(GREEN_CLR, "info", arg); 
      }

      static constexpr void LogDebug(const std::source_location& src_loc,
        const std::string& arg
      ) noexcept 
      {
        std::cout << FormatDebugMessage(src_loc, YELLOW_CLR, "debug", arg);
      }

      static constexpr void LogDebug(const std::string& arg) noexcept
      {
        std::cout << FormatMessage(YELLOW_CLR, "debug", arg);
      }

      static constexpr void LogError(const std::string& arg) noexcept 
      {
        std::cout << FormatMessage(RED_CLR, "error", arg);
      }

      static constexpr void LogWarning(const std::string& arg) noexcept 
      {
        std::cout << FormatMessage(BLUE_CLR, "warning", arg);
      }

      static constexpr void LogCritical(const std::string& arg) noexcept 
      {
        std::cout << FormatMessage(RED_CLR, "critical", arg);
      }

      static constexpr void LogToDo(const std::string& arg) noexcept 
      {
        std::cout << FormatMessage(MAGENTA_CLR, "todo", arg);
      }

    private:
      static std::string GetCurrentTimeAndDate()
      {
        const auto zt{ std::chrono::zoned_time{
          std::chrono::current_zone(),
          std::chrono::system_clock::now()
        }};
        return std::format("{:%F %H:%M:%OS}", zt);
      }
      
      static std::string FormatDebugMessage(const std::source_location& src_loc,
        const std::string color,
        const std::string& log_level,
        const std::string& msg
      ) noexcept
      {
        // 1 - date, 2 - color of level log, 3 - log level, 4 - reset color, 5 - argument
        return std::format("{}[{}] [{}] [{}]{}: {}\n",  
          color, 
          GetCurrentTimeAndDate(), 
          log_level, 
          src_loc.function_name(),
          RESET_CLR, 
          msg
        );
      }

      static std::string FormatMessage(
        const std::string color, 
        const std::string& log_level, 
        const std::string& arg
      ) noexcept
      {
        // 1 - date, 2 - color of level log, 3 - log level, 4 - reset color, 5 - argument
        return std::format("{}[{}] [{}]{}: {}\n", 
          color, 
          GetCurrentTimeAndDate(), 
          log_level, 
          RESET_CLR, 
          arg
        );
      }
    };

} // namespace avion::logger
