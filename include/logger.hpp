#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <mutex>
#include <chrono>
#include <iomanip>
#if defined(_WIN32)
#include <io.h>
#elif defined(__linux__)
#include <unistd.h>
#endif
#include <cstdlib>

#define LOG_DEBUG(msg, ...)   Logger::getInstance().log(LogLevel::DEBUG,   msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...)    Logger::getInstance().log(LogLevel::INFO,    msg, ##__VA_ARGS__)
#define LOG_WARNING(msg, ...) Logger::getInstance().log(LogLevel::WARNING, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...)   Logger::getInstance().log(LogLevel::ERROR,   msg, ##__VA_ARGS__)

enum class LogLevel
{
    DEBUG, INFO, WARNING, ERROR
};

class Logger
{
public:
    static Logger& getInstance()
    {
        static Logger instance;
        return instance;
    }

    // ✅ Fix 1: removido `static` — não pode acessar membros de instância sendo static
    template <typename... Args>
    void log(LogLevel level, std::string_view message, Args&&... args)
    {
        // ✅ Fix 2: era `logLevel`, variável inexistente — corrigido para `level`
        if(level < minLevel) return;

        std::lock_guard<std::mutex> lock(logMutex);
        printHeader(level);
        std::cout << message;
        // ✅ Fix 3: parêntese errado no fold expression e `args` faltando
        (std::cout << ... << std::forward<Args>(args));
        std::cout << "\n";
    }

    void setLevel(LogLevel level)
    {
        minLevel = level;
    }

private:
    Logger() : minLevel(LogLevel::INFO)
    {
    }

    // Singleton — sem cópia
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::mutex logMutex;
    LogLevel minLevel;

    void printHeader(LogLevel level)
    {
        // ✅ Fix 4: `now()` faltava os parênteses de chamada
        auto now = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now()
        );
        std::cout << "[" << std::put_time(std::localtime(&now), "%H:%M:%S") << "] ";

        if(supportColor())
        {
            switch(level)
            {
                case LogLevel::DEBUG:   std::cout << "\033[36m[DEBUG]\033[0m ";   break;
                case LogLevel::INFO:    std::cout << "\033[32m[INFO]\033[0m ";    break;
                case LogLevel::WARNING: std::cout << "\033[33m[WARN]\033[0m ";    break;
                case LogLevel::ERROR:   std::cout << "\033[31m[ERROR]\033[0m ";   break;
            }
        }
        else
        {
            switch(level)
            {
                case LogLevel::DEBUG:   std::cout << "[DEBUG] "; break;
                case LogLevel::INFO:    std::cout << "[INFO] ";  break;
                case LogLevel::WARNING: std::cout << "[WARN] ";  break;
                case LogLevel::ERROR:   std::cout << "[ERROR] "; break;
            }
        }
    }

    bool supportColor()
    {
#if defined(_WIN32)
        if(!_isatty(_fileno(stdout))) return false;
#elif defined(__linux__)
        if(!isatty(STDOUT_FILENO)) return false;
#endif
        const char* term = std::getenv("TERM");
        if(term == nullptr) return false;

        std::string t(term);
        return t != "dumb" && t != "emacs";
    }
};