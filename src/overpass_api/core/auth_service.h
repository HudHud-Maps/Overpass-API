/** Copyright 2026 Roland Olbricht et al.
 *
 * This file is part of Overpass_API.
 *
 * Overpass_API is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 */

#ifndef DE__OSM3S___OVERPASS_API__CORE__AUTH_SERVICE_H
#define DE__OSM3S___OVERPASS_API__CORE__AUTH_SERVICE_H

#include <string>
#include <map>

class Auth_Service {
public:
    Auth_Service();
    ~Auth_Service();

    // Returns user_id if valid, 0 otherwise
    int validate_key(const std::string& key);

private:
    std::string conn_info;
};

#endif
