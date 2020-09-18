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

#include "User.h"
#include "Group.h"
#include "../Utils/UUID.h"

Models::User::User(Group *group, uWS::WebSocket<false, true> *userWebsocket, bool isGroupLeader)
        : id(Utils::generateUUID()), group(group), userWebsocket(userWebsocket), isGroupLeader(isGroupLeader) {}

std::string Models::User::getId() { return id; }

bool Models::User::getIsGroupLeader() const { return isGroupLeader; }

void Models::User::handleMessage(std::string_view message) {
    if(this->getIsGroupLeader()) {
        switch ((int)message[0] - '0') {
            case MessageType::HostToSingleUser: {
                Models::User *user = this->group->getUser(message.substr(1, 23));
                if(user != nullptr) {
                    user->send(MessageType::HostToSingleUser, message);
                }
                break;
            }
            case MessageType::HostToAllUser:
                this->group->sendToAll(message);
                break;
        }
    } else {
        this->group->getHost()->send(MessageType::UserToHost, message);
    }
}

void Models::User::send(MessageType type, std::string_view message) {
    std::string messageString(message);
    this->userWebsocket->send(std::to_string(type) + messageString);
}

void Models::User::leave() {
    this->userWebsocket->end(3, "closed by host");
    this->group->removeUser(this->id);
}
