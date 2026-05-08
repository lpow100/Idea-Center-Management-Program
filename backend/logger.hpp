#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <chrono>
#include <fmt/chrono.h> 
#include <algorithm>
#include <pqxx/pqxx>

enum LogType {
    LOG_INFO, LOG_WARN, LOG_ERR
};

class Logger {
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getInstance() {
        static Logger instance; 
        return instance;
    }

    void SetChangeAuthor(int stembassadorID, pqxx::connection& database);

    void TerminalLog(LogType type, std::string message);

private:
    Logger() = default; 
    ~Logger() = default;
};


#endif