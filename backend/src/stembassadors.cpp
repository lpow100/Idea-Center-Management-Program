#include "stembassadors.hpp"
#include <random>

#include <iomanip>
#include <sstream>
#include <algorithm>

std::string CreateSessionID(int stembassadorID, pqxx::connection& database) {
    pqxx::work tx(database);

    uint8_t raw_bytes[32];
    std::random_device rd;
    std::generate(raw_bytes, raw_bytes + 32, std::ref(rd));

    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < 32; ++i) {
        ss << std::setw(2) << static_cast<int>(raw_bytes[i]);
    }
    std::string sessionToken = ss.str();
            
    tx.exec_params(
        "INSERT INTO sessions (token, stembassador_id) VALUES ($1, $2)",
        sessionToken, 
        stembassadorID
    );

    tx.commit();
    return sessionToken;
}

bool CheckSessionID(int stembassadorID, std::string sessionID, pqxx::connection& database) {
    // Use nontransaction for faster read-only lookups
    pqxx::nontransaction nt(database);

    pqxx::result result = nt.exec_params(
        "SELECT stembassador_id FROM sessions WHERE stembassador_id = $1 AND token = $2",
        stembassadorID,
        sessionID
    );

    nt.commit();

    return !result.empty();
}

int GetStembassadorID(std::string name, pqxx::connection& database) {
    pqxx::nontransaction nt(database);

    pqxx::result result = nt.exec_params(
        "SELECT id FROM stembassadors WHERE name = $1",
        name
    );

    nt.commit();

    return result[0]["id"].as<int>();
}
