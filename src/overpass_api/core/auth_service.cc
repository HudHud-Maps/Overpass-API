/** Copyright 2026 Roland Olbricht et al.
 *
 * This file is part of Overpass_API.
 *
 * Overpass_API is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 */

#include "auth_service.h"
#include <cstdlib>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if defined(HAVE_LIBPQ_FE_H)
#include <libpq-fe.h>
#elif defined(HAVE_POSTGRESQL_LIBPQ_FE_H)
#include <postgresql/libpq-fe.h>
#else
// Fallback if autoconf headers are not yet fully propagated
#include <postgresql/libpq-fe.h>
#endif

Auth_Service::Auth_Service() {
    const char* db_host = getenv("OVERPASS_AUTH_DB_HOST");
    const char* db_port = getenv("OVERPASS_AUTH_DB_PORT");
    const char* db_name = getenv("OVERPASS_AUTH_DB_NAME");
    const char* db_user = getenv("OVERPASS_AUTH_DB_USER");
    const char* db_pass = getenv("OVERPASS_AUTH_DB_PASS");

    // Default connection string if env vars are missing (for testing)
    conn_info = "";
    if (db_host) conn_info += "host=" + std::string(db_host) + " ";
    if (db_port) conn_info += "port=" + std::string(db_port) + " ";
    if (db_name) conn_info += "dbname=" + std::string(db_name) + " ";
    if (db_user) conn_info += "user=" + std::string(db_user) + " ";
    if (db_pass) conn_info += "password=" + std::string(db_pass) + " ";
}

Auth_Service::~Auth_Service() {}

int Auth_Service::validate_key(const std::string& key) {
    if (key.empty()) return 0;

    PGconn* conn = PQconnectdb(conn_info.c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        PQfinish(conn);
        return 0;
    }

    const char* paramValues[1];
    paramValues[0] = key.c_str();

    PGresult* res = PQexecParams(conn,
        "SELECT id FROM users WHERE api_key = $1",
        1,       /* one param */
        NULL,    /* let the backend deduce param type */
        paramValues,
        NULL,    /* param lengths not needed for text */
        NULL,    /* default to all text params */
        0);      /* ask for text results */

    int user_id = 0;
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        user_id = atoi(PQgetvalue(res, 0, 0));
    }

    PQclear(res);
    PQfinish(conn);

    return user_id;
}
