#include <iostream>
#include <optional>
#include <pqxx/pqxx>
#include <crow.h>
#include <crow/json.h>
#include <argon2.h>
#include <algorithm>
#include <dotenv.h>
#include "logger.hpp"
#include "stembassadors.hpp"

#define HASHLEN 32
#define SALTLEN 16
#define ENCODED_LEN 128 


int main() {
    dotenv env(".env");  // Load variables from .env file

    // Retrieve variables with default values if they are not set
    std::string db_host = env.get("DB_HOST", "localhost");
    std::string db_user = env.get("DB_USER", "postgres");
    std::string db_password = env.get("DB_PASSWORD", "");

    crow::SimpleApp app;
    std::optional<pqxx::connection> database_definition;

    try {
        database_definition = pqxx::connection("user=" + db_user + " password=" + db_password + " host=" + db_host + " dbname=ideacenterdowntowninventory");
    } catch (const std::exception &e) {
        std::cerr << "[ERROR]: Could not init datasbase: \n\t" << e.what() << std::endl;
        return 1;
    }

    if (!database_definition.has_value()) {
        std::cerr << "[ERROR]: Could not init datasbase: \n\tUnkown Reason" << std::endl;
        return 1;
    }

    pqxx::connection& database = database_definition.value();

    CROW_ROUTE(app, "/modify-class-members/").methods(crow::HTTPMethod::POST)
    ([&database](const crow::request& req) {
        auto x = crow::json::load(req.body);
        
        if (!x) {
            return crow::response(400, "Invalid JSON");
        }

        pqxx::work tx(database);
        pqxx::result dbResult = tx.exec("SELECT id, name FROM ideas_kits");

        crow::json::wvalue response;
        response["status"] = "success";

        for (auto const &row : dbResult) {
            std::cout << "ID: " << row["id"].as<int>() 
                      << " Name: " << row["name"].as<std::string>() << std::endl;
            response["message"] = "Name: " + row["name"].as<std::string>();
        }
        
        tx.commit();

        return crow::response(response);
    });

    CROW_ROUTE(app, "/add-kit/").methods(crow::HTTPMethod::POST)
    ([&database](const crow::request& req) {
        auto x = crow::json::load(req.body);

        if (!x) {
            return crow::response(400, "Invalid JSON");
        }

        int stembassadorID = GetStembassadorID((std::string)x["stembassador"].s(),database);

        if (!CheckSessionID(stembassadorID,(std::string)x["session-id"].s(),database)) {
            return crow::response(400, "Invalid Credentials");
        }

        crow::json::rvalue kitInfo = x["kit-info"];

        if (!kitInfo) {
            return crow::response(400, "Invalid kit info");
        }

        // get kit params

        std::string kitName = std::string(kitInfo["name"].s());
        int kitLength = int(kitInfo["length"].i());
        int kitPrice = int(kitInfo["price"].i());
        int kitStock = int(kitInfo["stock"].i());
        try {
            Logger::getInstance().SetChangeAuthor(stembassadorID,database);

            pqxx::work tx(database);
            
            tx.exec_params(
                "INSERT INTO idea_kits (author, name, lengthMs, price, stock) VALUES ($1, $2, $3, $4, $5)",
                stembassadorID, kitName, kitLength, kitPrice, kitStock
            );

            tx.commit();

            crow::json::wvalue response;
            response["status"] = "success";
            response["message"] = "Added new IDEA kit";
            return crow::response(200, response);
        } catch (const std::exception &e) {
            std::cerr << "Database error: " << e.what() << std::endl;
            crow::json::wvalue responseJson;
            responseJson["status"] = "error";
            responseJson["message"] = "Failed to add to database!";
            return crow::response(500, responseJson);
        }
    });

    CROW_ROUTE(app, "/create-stembassador/").methods(crow::HTTPMethod::POST)
    ([&database](const crow::request& req) {
        auto body_json = crow::json::load(req.body);
        
        if (!body_json) {
            return crow::response(400, "Invalid JSON");
        }

        int stembassadorID = GetStembassadorID((std::string)body_json["stembassador"].s(),database);

        if (!CheckSessionID(stembassadorID,(std::string)body_json["session-id"].s(),database)) {
            return crow::response(400, "Invalid Credentials");
        }

        char hash1[ENCODED_LEN];

        uint8_t salt[SALTLEN];
        std::random_device rd;
        std::generate(salt, salt + SALTLEN, std::ref(rd));

        uint8_t *pwd = (uint8_t *)strdup(((std::string)body_json["password"].s()).c_str());
        uint32_t pwdlen = strlen((char *)pwd);

        uint32_t t_cost = 4;            // 1-pass computation
        uint32_t m_cost = (1<<16);      // 64 mebibytes memory usage
        uint32_t parallelism = 2;       // number of threads and lanes

        int result = argon2id_hash_encoded(t_cost, m_cost, parallelism, pwd, pwdlen, salt, SALTLEN, HASHLEN, hash1, ENCODED_LEN);

        // high-level API
        if (result != ARGON2_OK) {
            std::cerr << "Error: " << argon2_error_message(result) << std::endl;
            crow::json::wvalue responseJson;
            responseJson["status"] = "error";
            responseJson["message"] = "Could not secure password!";
            return crow::response(500, responseJson);
        }

        try {
            Logger::getInstance().SetChangeAuthor(stembassadorID,database);

            pqxx::work tx(database);
            
            tx.exec_params(
                "INSERT INTO stembassadors (name, password) VALUES ($1, $2)",
                std::string(body_json["name"].s()), 
                std::string(hash1)
            );

            tx.commit();

            crow::json::wvalue response;
            response["status"] = "success";
            response["message"] = "Added new stembassador";
            return crow::response(200, response);

        } catch (const std::exception &e) {
            std::cerr << "Database error: " << e.what() << std::endl;
            crow::json::wvalue responseJson;
            responseJson["status"] = "error";
            responseJson["message"] = "Failed to add to database!";
            return crow::response(500, responseJson);
        }
    });

    CROW_ROUTE(app, "/login-stembassador/").methods(crow::HTTPMethod::POST)
    ([&database](const crow::request& req) {
        auto body_json = crow::json::load(req.body);
        
        if (!body_json) {
            return crow::response(400, "Invalid JSON");
        }

        int stembassadorID = -1;

        try {
            pqxx::work tx(database);
            
            pqxx::result databaseResult = tx.exec_params(
                "SELECT id, password FROM stembassadors WHERE name = ($1)",
                std::string(body_json["name"].s())
            );

            tx.commit();

            int result = argon2_verify(
                databaseResult[0]["password"].as<std::string>().c_str(), 
                ((std::string)body_json["password"]).c_str(), 
                ((std::string)body_json["password"]).length(), 
                Argon2_id
            );

            if (result != ARGON2_OK) {
                crow::json::wvalue responseJson;
                responseJson["status"] = "error";
                responseJson["message"] = "Invalid username or password!";
                return crow::response(500, responseJson);
            }

            stembassadorID = databaseResult[0]["id"].as<int>();
        } catch (const std::exception &e) {
            std::cerr << "Database error: " << e.what() << std::endl;
            crow::json::wvalue responseJson;
            responseJson["status"] = "error";
            responseJson["message"] = "Failed to add to database!";
            return crow::response(500, responseJson);
        }

        std::string sessionID = CreateSessionID(stembassadorID, database);

        crow::json::wvalue response;
        response["status"] = "success";
        response["message"] = "Logged in";
        response["session-id"] = sessionID;

        return crow::response(200, response);
    });

    CROW_ROUTE(app, "/remove-stembassador/").methods(crow::HTTPMethod::POST)
    ([&database](const crow::request& req) {
        auto x = crow::json::load(req.body);

        if (!x) {
            return crow::response(400, "Invalid JSON");
        }

        int stembassadorID = GetStembassadorID((std::string)x["stembassador"].s(),database);

        if (!CheckSessionID(stembassadorID,(std::string)x["session-id"].s(),database)) {
            return crow::response(400, "Invalid Credentials");
        }

        try {
            pqxx::transaction tx(database);

            tx.exec_params(
                "ALTER TABLE stembassadors DROP ROW WHERE name = $1",
                (std::string)x["to-remove"].s()
            );

            tx.commit();


            crow::json::wvalue responseJson;
            responseJson["status"] = "success";
            responseJson["message"] = "Removed Stembassador";
            return crow::response(500, responseJson);
        } catch (const std::exception &e) {
            std::cerr << "Database error: " << e.what() << std::endl;
            crow::json::wvalue responseJson;
            responseJson["status"] = "error";
            responseJson["message"] = "Failed to add to database!";
            return crow::response(500, responseJson);
        }
    });

    CROW_ROUTE(app, "/find-user/<string>").methods(crow::HTTPMethod::GET)
    ([&database](const crow::request& req, std::string name) {
        
    });

    app.port(18080).run();

    database.close();
}

/*
int main(int argc, char** argv) {

    try {
        // Connect to the database
        
        // Start a transaction
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
*/