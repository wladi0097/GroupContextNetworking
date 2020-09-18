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
#include "GroupManager.h"
#include <vector>
#include <memory>


Models::User *GroupManager::joinNewGroup(uWS::WebSocket<false, true> *userWebsocket) {
    std::unique_ptr<Models::Group> group = std::make_unique<Models::Group>(userWebsocket);
    groups.push_back(std::move(group));
    Models::Group *newGroup = groups.back().get();

    return newGroup->getHost();
}

Models::User *
GroupManager::joinGroup(Models::Group *group, uWS::WebSocket<false, true> *userWebsocket, bool isHost = false) {
    auto *user = group->addUser(userWebsocket, isHost);
    group->getHost()->send(MessageType::newUser ,user->getId());
    return user;
}

Models::Group *GroupManager::getGroup(const std::string &groupId) {
    for (std::unique_ptr<Models::Group> &group : groups) {
        if (group->getId() == groupId) {
            return group.get();
        }
    }

    return nullptr;
}

void GroupManager::leaveGroup(Models::User *user) {
    if (user->getIsGroupLeader()) {
        this->removeGroup();
    } else {
        user->leave();
    }
}

void GroupManager::removeGroup(const std::string &groupId) {
    const int16_t index = getGroupIndex(groupId);
    if (index != -1) {
        groups.erase(groups.begin() + index);
    }
}

int16_t GroupManager::getGroupIndex(const std::string &groupId) {
    for (int16_t i = 0; i < groups.size(); ++i) {
        if (groups[i]->getId() == groupId)
            return i;
    }
    return -1;
}
