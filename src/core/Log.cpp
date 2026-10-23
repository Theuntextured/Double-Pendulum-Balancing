#include "Log.h"

#include <filesystem>
#include <iostream>

LogGroup::LogGroup(const std::string &name, LogType_t defaultType) {
    std::lock_guard lock(logMutex);

    m_name = name;
    m_defaultType = defaultType;
    logGroups.emplace(name, this);

    std::filesystem::create_directories("logs");
    logFile = std::ofstream(std::string("logs/") + name + ".log");
    if (!logFile) {
        std::cerr << "Failed to open or create log file " << name << ".log" << std::endl;
        isLogFileOpen = false;
    }
    else isLogFileOpen = true;
}


void LogGroup::logMessage(const std::string &message, LogType_t type) const {
    if (type == LogType::Default) type = m_defaultType;
    std::lock_guard lock(logMutex);

    const std::time_t now = std::time(nullptr);
    std::tm local{};

    #ifdef _WIN32
        localtime_s(&local, &now);
    #else
        localtime_r(&now, &local);
    #endif
    const auto timeString = std::put_time(&local, "%Y-%m-%d %H:%M:%S");

    if (type & enabledLogTypes && m_enabled) {
        std::cout
            << getLogColour(type)
            << "[" << timeString    << "]"
            << "[" << m_name        << "] "
            << getLogTypeText(type)
            << ": " << message
            << Colour::ansiReset
            << std::endl;
    }
    if (isLogFileOpen)
        logFile
            << "[" << timeString << "] "
            << getLogTypeText(type)
            << ": " << message
            << std::endl;
}

void LogGroup::setEnabled(bool enabled) {
    std::lock_guard lock(logMutex);
    m_enabled = enabled;
}

void LogGroup::setEnabledLogTypes(uint8_t types) {
    std::lock_guard lock(logMutex);
    enabledLogTypes = types;
}

void LogGroup::setLogTypeEnabled(LogType_t type, bool enabled) {
    std::lock_guard lock(logMutex);
    if (enabled) enabledLogTypes |= type;
    else enabledLogTypes &= ~type;
}

LogGroup * LogGroup::getLogGroup(const std::string &name) {
    std::lock_guard lock(logMutex);
    if (const auto found = logGroups.find(name); found != logGroups.end())
        return found->second;
    return nullptr;
}

const std::unordered_map<std::string, LogGroup *> & LogGroup::getLogGroups() {
    return logGroups;
}

// must be first in CMakeLists.txt to ensure this stuff is initialized.

uint8_t LogGroup::enabledLogTypes = 0xFF;
std::mutex LogGroup::logMutex;
std::unordered_map<std::string, LogGroup*> LogGroup::logGroups;

DEFINE_LOG_GROUP(LogTemp, Info)
