#pragma once 

#include "logger/logger.hpp"

#define AV_LOGGER               avion::logger::Logger
#define AV_LOG(a, b)            AV_LOGGER::Log(a, b)

#define AV_LOG_LEVEL            avion::logger::LogLevel

#define AV_LOG_LEVEL_INFO       AV_LOG_LEVEL::kInformation
#define AV_LOG_LEVEL_WARNING    AV_LOG_LEVEL::kWarning
#define AV_LOG_LEVEL_ERROR      AV_LOG_LEVEL::kError
#define AV_LOG_LEVEL_DEBUG      AV_LOG_LEVEL::kDebug
#define AV_LOG_LEVEL_CRITICAL   AV_LOG_LEVEL::kCritical

#define AV_LOG_INFO(msg)        do { AV_LOG(AV_LOG_LEVEL_INFO, msg); }      while(0);
#define AV_LOG_WARNING(msg)     do { AV_LOG(AV_LOG_LEVEL_DEBUG, msg); }     while(0);
#define AV_LOG_ERROR(msg)       do { AV_LOG(AV_LOG_LEVEL_ERROR, msg); }     while(0);
#define AV_LOG_DEBUG(msg)       do { AV_LOG(AV_LOG_LEVEL_DEBUG, msg); }     while(0);
#define AV_LOG_CRITICAL(msg)    do { AV_LOG(AV_LOG_LEVEL_CRITICAL, msg); }  while(0);
