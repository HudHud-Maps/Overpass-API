/** Copyright 2026 Roland Olbricht et al.
 *
 * This file is part of Overpass_API.
 *
 * Overpass_API is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 */

#ifndef DE__OSM3S___OVERPASS_API__CORE__QUERY_LOGGER_H
#define DE__OSM3S___OVERPASS_API__CORE__QUERY_LOGGER_H

#include <string>

class Query_Logger {
public:
    Query_Logger();
    ~Query_Logger();

    void log_query(int user_id, const std::string& query, const std::string& ip_address,
                   const std::string& client_name);

    static std::string detect_client_name();

private:
    std::string conn_info;
};

#endif
