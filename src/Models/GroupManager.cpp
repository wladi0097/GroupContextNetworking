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


Models::Group *GroupManager::joinNewGroup() {
    std::unique_ptr<Models::Group> group = std::make_unique<Models::Group>();
    groups.push_back(std::move(group));
    Models::Group *newGroup = groups.back().get();

    return newGroup;
}

Models::User *
GroupManager::joinGroup(Models::Group *group, bool isHost = false) {
    auto *user = group->addUser(isHost);
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

void GroupManager::removeGroup(Models::Group *group) {
    group->closeWebSocketSessionForAllUsers();

    const int16_t index = getGroupIndex(group->getId());
    if (index != -1) {
        groups.erase(groups.begin() + index);
    }
}

int16_t GroupManager::getGroupIndex(const std::string &groupId) {
    for (unsigned long i = 0; i < groups.size(); ++i) {
        if (groups[i]->getId() == groupId)
            return i;
    }
    return -1;
}

void GroupManager::handleMessage(UserData *userdata, std::string_view message) {
    if(userdata->user->getIsGroupLeader()) {
        switch ((int)message[0] - '0') {
            case MessageType::HostToSingleUser: {
                Models::User *user = userdata->group->getUser(message.substr(1, 23));
                if(user != nullptr) {
                    user->send(MessageType::HostToSingleUser, message);
                }
                break;
            }
            case MessageType::HostToAllUser:
                userdata->group->sendToAll(message);
                break;
        }
    } else {
        userdata->group->getHost()->send(MessageType::UserToHost, message);
    }
}

void GroupManager::handleLeave(UserData *userdata) {
    if (userdata->user->getIsGroupLeader()) {
        this->removeGroup(userdata->group);
    } else {
        userdata->group->removeUser(userdata->user);
    }
}