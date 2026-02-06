/** Copyright 2026 Roland Olbricht et al.
 *
 * This file is part of Overpass_API.
 *
 * Overpass_API is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 */

#include "query_logger.h"
#include <cstdlib>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if defined(HAVE_LIBPQ_FE_H)
#include <libpq-fe.h>
#elif defined(HAVE_POSTGRESQL_LIBPQ_FE_H)
#include <postgresql/libpq-fe.h>
#else
#include <postgresql/libpq-fe.h>
#endif

Query_Logger::Query_Logger() {
    const char* db_host = getenv("OVERPASS_AUTH_DB_HOST");
    const char* db_port = getenv("OVERPASS_AUTH_DB_PORT");
    const char* db_name = getenv("OVERPASS_AUTH_DB_NAME");
    const char* db_user = getenv("OVERPASS_AUTH_DB_USER");
    const char* db_pass = getenv("OVERPASS_AUTH_DB_PASS");

    conn_info = "";
    if (db_host) conn_info += "host=" + std::string(db_host) + " ";
    if (db_port) conn_info += "port=" + std::string(db_port) + " ";
    if (db_name) conn_info += "dbname=" + std::string(db_name) + " ";
    if (db_user) conn_info += "user=" + std::string(db_user) + " ";
    if (db_pass) conn_info += "password=" + std::string(db_pass) + " ";
}

Query_Logger::~Query_Logger() {}

void Query_Logger::log_query(int user_id, const std::string& query, const std::string& ip_address) {
    PGconn* conn = PQconnectdb(conn_info.c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        PQfinish(conn);
        return;
    }

    std::string user_id_str = std::to_string(user_id);
    const char* paramValues[3];
    paramValues[0] = user_id_str.c_str();
    paramValues[1] = query.c_str();
    paramValues[2] = ip_address.c_str();

    PGresult* res = PQexecParams(conn,
        "INSERT INTO query_logs (user_id, query_text, ip_address) VALUES ($1, $2, $3)",
        3,
        NULL,
        paramValues,
        NULL,
        NULL,
        0);

    PQclear(res);
    PQfinish(conn);
}
