#pragma once
#include "User.h"

struct UserData {
    std::string_view path;
    Models::User *user;
    Models::Group *group;
};