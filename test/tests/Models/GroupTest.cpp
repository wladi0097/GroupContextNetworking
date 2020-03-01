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
#include "User.h"
#include "Group.h"

using namespace Models;

TEST_CASE("Group: Constructor") {
    SECTION("has unique id") {
        auto* group = new Group;
        REQUIRE(!empty(group->getId()));
    }
}

TEST_CASE("Group: Adding User") {
    SECTION("Single User") {
        auto* group = new Group;

        auto user = group->addUser();

        REQUIRE(group->getUser(user->getId()) == user);
    }

    SECTION("Multiple Users") {
        auto* group = new Group;

        auto* user1 = group->addUser();
        auto* user2 = group->addUser();

        REQUIRE(group->getUser(user1->getId()) == user1);
        REQUIRE(group->getUser(user2->getId()) == user2);
    }
}

TEST_CASE("Group: Getting Users") {
    SECTION("Get by id found") {
        auto* group = new Group;

        auto* user = group->addUser();

        REQUIRE(group->getUser(user->getId()) == user);
    }

    SECTION("Get by id not found") {
        auto* group = new Group;

        REQUIRE(!group->getUser("random-id"));
    }

    SECTION("Has user true") {
        auto* group = new Group;

        auto* user = group->addUser();

        REQUIRE(group->hasUser(user->getId()));
    }

    SECTION("Has user false") {
        auto* group = new Group;

        REQUIRE(!group->hasUser("random-id"));
    }
}

TEST_CASE("Group: Removing Users") {
    SECTION("Remove user also destructor the user") {
        auto* group = new Group;

        auto* user = group->addUser();

        REQUIRE(group->getUser(user->getId()) == user);

        group->removeUser(user->getId());

        //REQUIRE(empty(group->()));
    }

    SECTION("Remove user that doesn't exist is ignored") {
        auto* group = new Group;
        group->removeUser("random-id");
    }
}
