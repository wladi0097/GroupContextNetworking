/*
GroupContextNetworking by wladi0097

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Wladislaw Koschemako.
Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Group.h"

using namespace Models;

Group::Group() {
    this->id = Utils::generateUUID();
}

Group::~Group() = default;

std::vector<User *> Group::getUsers() {
    return this->users;
}

std::string Group::getId() {
    return this->id;
}

User *Group::getUser(const std::string &userId) {
    for (User *user : this->users) {
        if (user->getId() == userId)
            return user;
    }
    return nullptr;
}

void Group::addUser(User *user) {
    this->users.push_back(user);
}

void Group::removeUser(const std::string &userId) {
    int index = this->getUserIndex(userId);
    if (index != -1) {
        User* user = this->users[index];
        this->users.erase(this->users.begin() + index);
        delete user;
    }
}

int Group::getUserIndex(const std::string &userId) {
    for (__int16_t i = 0; i < this->users.size(); ++i) {
        if (users[i]->getId() == userId)
            return i;
    }
    return -1;
}

bool Group::hasUser(const std::string &userId) {
    return this->getUser(userId) != nullptr;
}

bool Group::isUserGroupCreator(const std::string &userId) {
    User* user = this->getUser(userId);

    if (user == nullptr) return false;

    return user->getIsGroupLeader();
}
