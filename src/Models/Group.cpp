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
#include <memory>
#include <vector>
#include "../Utils/UUID.h"

Models::Group::Group() {
    this->id = "AAAA"; // Utils::generateUUID()
    this->host = std::make_unique<Models::User>(this, true);
}

std::string Models::Group::getId() {
    return id;
}

Models::User *Models::Group::getHost() {
    return this->host.get();
}

Models::User *Models::Group::getUser(const std::string_view &userId) {
    for (std::unique_ptr<User> &user : users) {
        if (user->getId() == userId)
            return user.get();
    }
    return nullptr;
}

Models::User *Models::Group::addUser(const bool isCreator) {
    std::unique_ptr<Models::User> user =
            std::make_unique<Models::User>(this, isCreator);
    users.push_back(std::move(user));
    return users.back().get();
}

void Models::Group::removeUser(Models::User *user) {
    user->disconnect();

    int index = getUserIndex(user->getId());
    if (index != -1) {
        users.erase(users.begin() + index);
    }
}

int Models::Group::getUserIndex(const std::string &userId) {
    for (unsigned long i = 0; i < users.size(); ++i) {
        if (users[i]->getId() == userId)
            return i;
    }
    return -1;
}

void Models::Group::sendToAll(std::string_view message) {
    for (auto &user: this->users) {
        user->send(MessageType::HostToAllUser, message);
    }
}

void Models::Group::closeWebSocketSessionForAllUsers() {
    for (auto &user: this->users) {
        user->disconnect();
    }
}
