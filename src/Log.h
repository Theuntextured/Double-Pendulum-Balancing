#pragma once

#include <fstream>
#include <mutex>
#include <unordered_map>
#include <string>
#include <string_view>
#include <sstream>

#include "Colour.h"
#include "Common.h"

namespace LogType {
    enum Type : int8_t {
        Default = INDEX_NONE,
        Debug = 1,
        Info = 2,
        Warning = 4,
        Error = 8,
        Fatal = 16,
        Success = 32
    };
}
using LogType_t = LogType::Type;

static constexpr Colour getLogColour(LogType_t type) {
    switch (type) {
        case LogType::Debug: return Colour::Grey;
        case LogType::Info: return Colour::White;
        case LogType::Warning: return Colour::Orange;
        case LogType::Error: return Colour::Red;
        case LogType::Fatal: return Colour::BrightRed;
        case LogType::Success: return Colour::Green;
        default: return Colour::LightGrey;
    }
}
static constexpr std::string_view getLogTypeText(LogType_t type) {
    switch (type) {
        case LogType::Default: return "Default";
        case LogType::Debug: return "Debug";
        case LogType::Info: return "Info";
        case LogType::Warning: return "Warning";
        case LogType::Error: return "Error";
        case LogType::Fatal: return "Fatal";
        case LogType::Success: return "Success";
        default: return "Unknown";
    }
}

class LogGroup {
public:
    explicit LogGroup(const std::string& name, LogType_t defaultType = LogType::Info);
    
    void logMessage(const std::string& message, LogType_t type = LogType::Default) const;

    template<typename... ArgsType>
        requires (sizeof...(ArgsType) > 0)
    LogGroup &operator<<(const ArgsType &... args) {
        std::ostringstream stream;
        bool first = true;

        ((stream << (first ? "" : " ") << args, first = false), ...);

        logMessage(stream.str());

        return *this;
    }
    template<typename... ArgsType>
        requires (sizeof...(ArgsType) > 0)
    void logMessage(LogType_t type, const ArgsType &... args) {
        std::ostringstream stream;
        bool first = true;

        ((stream << (first ? "" : " ") << args, first = false), ...);

        logMessage(stream.str(), type);
    }

    NODISCARD const std::string& getName() const { return m_name; }
    NODISCARD LogType_t getDefaultType() const { return m_defaultType; }
    NODISCARD bool isEnabled() const { return m_enabled; }
    
    void setEnabled(bool enabled);
    static void setEnabledLogTypes(uint8_t types);
    static void setLogTypeEnabled(LogType_t type, bool enabled);

    static LogGroup* getLogGroup(const std::string& name);
    static const std::unordered_map<std::string, LogGroup*>& getLogGroups();
private:
    std::string m_name;
    LogType_t m_defaultType = LogType::Info;
    bool m_enabled = true;

    static std::mutex logMutex;
    static uint8_t enabledLogTypes;
    static std::unordered_map<std::string, LogGroup*> logGroups;

    mutable std::ofstream logFile;
    bool isLogFileOpen;
};

#define LOG(Group, Verbosity, ...) \
    GlobalLogGroups::g_##Group.logMessage(LogType::Verbosity, __VA_ARGS__)
#define DECLARE_LOG_GROUP(Name) namespace GlobalLogGroups { extern LogGroup g_##Name;}
#define DEFINE_LOG_GROUP(Name, DefaultVerbosity) namespace GlobalLogGroups {LogGroup g_##Name(STRINGIFY(Name), LogType::DefaultVerbosity);}

DECLARE_LOG_GROUP(LogTemp)