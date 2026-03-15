#pragma once

#include <iostream>

namespace avion::logger {
    enum class LogLevel {
        kInformation = 0,
        kDebug = 1,
        kWarning = 2,
        kError = 3,
        kCritical = 4,
    };

    class Logger {
        public:
            Logger() = delete;

            static constexpr void Log(LogLevel level, const std::string& arg) noexcept {
                switch (level) {
                    case LogLevel::kInformation:
                        LogInfo(arg);
                        break;
                    case LogLevel::kDebug:
                        LogDebug(arg);
                        break;
                    case LogLevel::kError:
                        LogError(arg);
                        break;
                }
            }

            static constexpr void LogInfo(const std::string& arg) noexcept {
                std::cout << "[info]: " << arg << '\n';
            }

            static constexpr void LogDebug(const std::string& arg) noexcept {
                std::cout << "[debug]: " << arg << '\n';
            }

            static constexpr void LogError(const std::string& arg) noexcept {
                std::cerr << "[error]: " << arg << '\n';
            }
        };

} // namespace avion::logger
