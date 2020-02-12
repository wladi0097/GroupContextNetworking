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

Models::User* GroupManager::joinGroup() {
    return this->createGroupWithUser();
}

Models::User* GroupManager::joinGroup(const std::string &groupId) {
    Models::Group* group = this->getGroup(groupId);
    if (group) {
        auto* user = new Models::User(group);
        return user;
    } else {
        return this->createGroupWithUser();
    }
}

Models::Group* GroupManager::getGroup(const std::string &groupId) {
    for (Models::Group *group : this->groups) {
        if (group->getId() == groupId) {
            return group;
        }
    }

    return nullptr;
}

Models::User* GroupManager::createGroupWithUser() {
    auto* group = new Models::Group;
    this->groups.push_back(group);

    auto* user = new Models::User(group, true);
    return user;
}

void GroupManager::removeGroup(const std::string &groupId) {
    const int index = this->getGroupIndex(groupId);
    if (index != -1) {
        Models::Group* group = this->groups[index];
        group->removeAllUsers();

        this->groups.erase(this->groups.begin() + index);
        delete group;
    }
}

int GroupManager::getGroupIndex(const std::string &groupId) {
    for (__int16_t i = 0; i < this->groups.size(); ++i) {
        if (groups[i]->getId() == groupId)
            return i;
    }
    return -1;
}
