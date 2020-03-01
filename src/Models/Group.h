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


#ifndef NET_GROUP_H
#define NET_GROUP_H

#include <vector>
#include <string>
#include <memory>

#include "User.h"
#include "../Utils/UUID.h"

namespace Models {
class Group {
 private:
    std::vector<std::unique_ptr<User>> users;
    std::string id;
    int getUserIndex(const std::string& id);

 public:
    Group();
    ~Group();
    std::string getId();
    User* getUser(const std::string& userId);
    User* addUser();
    void removeUser(const std::string& userId);
    bool hasUser(const std::string& userId);
    __int16_t getUserSize();
    bool isUserGroupCreator(const std::string& userId);
};
}  // namespace Models

#endif  // NET_GROUP_H
