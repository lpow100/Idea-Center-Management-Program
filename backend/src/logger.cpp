#include "logger.hpp"

void Logger::SetChangeAuthor(int stembassadorID, pqxx::connection& database) {
    pqxx::work tx(database);
    
    tx.exec_params(
        "SELECT set_config('app.current_stembassador_id', $1, false)",
        std::to_string(stembassadorID)
    );

    tx.commit();
}

void Logger::TerminalLog(LogType type, std::string message) {
    auto now = std::chrono::system_clock::now();
    std::string logTypeString = "[LOG_UNKNOWN]";
    switch (type)   {
        case LOG_INFO: logTypeString = "[LOG_INFO]"; break;
        case LOG_WARN: logTypeString = "[LOG_WARN]"; break;
        case LOG_ERR:  logTypeString = "[LOG_ERR]";  break;
        
        default:
            break;
    }
    std::cout << fmt::format("{:%Y-%m-%d %H:%M:%S}", now) << " " << logTypeString << " " << message << "\n";
}