#ifndef STEMBASSADORS_HPP
#define STEMBASSADORS_HPP

#include <iostream>
#include <optional>
#include <pqxx/pqxx>
#include <argon2.h>
#include <algorithm>

/*
 * Creates a session ID for a stembassador and stores it in the DB
 * @param stembassadorID The ID for the stembassador to create a session ID for
 * @result a `std::string` with the session ID for the stembassador
*/
std::string CreateSessionID(int stembassadorID, pqxx::connection& database);

/*
 * Creates a session ID for a stembassador and stores it in the DB
 * @param stembassadorID The ID for the stembassador to create a session ID for
 * @param sessionID The sessiond ID of the logged in stembassador
 * @result a `bool` if true then the session ID is correct
*/
bool CheckSessionID(int stembassadorID, std::string sessionID, pqxx::connection& database);

int GetStembassadorID(std::string name, pqxx::connection& database);

#endif