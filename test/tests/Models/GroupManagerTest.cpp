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

#include "../../../deps/catch.hpp"
#include "../../../src/Models/GroupManager.h"

TEST_CASE("GroupManager: join") {
    SECTION("Just joining will create new group with user inside") {
        std::unique_ptr<GroupManager> groupManager =
                std::make_unique<GroupManager>();
        auto *user = groupManager->joinGroup();
        auto *group = user->getGroup();

        REQUIRE(!user->getId().empty());
        REQUIRE(!group->getId().empty());
        REQUIRE(group->getUser(user->getId())->getId() == user->getId());
    }

    SECTION("Joining an existing group will create and add a user") {
        std::unique_ptr<GroupManager> groupManager =
                std::make_unique<GroupManager>();
        auto *user1 = groupManager->joinGroup();
        auto *group = user1->getGroup();
        auto *user2 = groupManager->joinGroup(group->getId());

        REQUIRE(group->getUserSize() == 2);
        REQUIRE(user1->getIsGroupLeader());
        REQUIRE(!user2->getIsGroupLeader());
    }

    SECTION("Joining a non existent group will create one") {
        std::unique_ptr<GroupManager> groupManager =
                std::make_unique<GroupManager>();
        auto *user = groupManager->joinGroup("IDoNotExist");
        auto *group = user->getGroup();

        REQUIRE(!user->getId().empty());
        REQUIRE(!group->getId().empty());
        REQUIRE(groupManager->getGroupSize() == 1);
        REQUIRE(group->getUser(user->getId())->getId() == user->getId());
    }

    SECTION("Multiple groups can exist") {
        std::unique_ptr<GroupManager> groupManager =
                std::make_unique<GroupManager>();
        auto *user1 = groupManager->joinGroup();
        auto *user2 = groupManager->joinGroup();
        auto *user3 = groupManager->joinGroup();

        REQUIRE(user1->getId() != user2->getId());
        REQUIRE(user2->getId() != user3->getId());
        REQUIRE(user1->getGroup()->getId() != user2->getGroup()->getId());
        REQUIRE(user2->getGroup()->getId() != user3->getGroup()->getId());
        REQUIRE(groupManager->getGroupSize() == 3);
    }
}

TEST_CASE("GroupManager: leave") {
    SECTION("One user leaves group will just remove that one user") {
        std::unique_ptr<GroupManager> groupManager =
                std::make_unique<GroupManager>();
        auto *user1 = groupManager->joinGroup();
        REQUIRE(user1->getIsGroupLeader());

        auto *group = user1->getGroup();
        auto *user2 = groupManager->joinGroup(group->getId());
        REQUIRE(!user2->getIsGroupLeader());

        groupManager->leaveGroup(user1);

        REQUIRE(group->getUserSize() == 1);
        REQUIRE(user2->getIsGroupLeader());
    }

    SECTION("Last user leaves group, deletes it") {
        std::unique_ptr<GroupManager> groupManager =
                std::make_unique<GroupManager>();
        auto *user = groupManager->joinGroup();
        auto *group = user->getGroup();

        groupManager->leaveGroup(user);

        REQUIRE(groupManager->getGroupSize() == 0);
    }
}
