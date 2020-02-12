#include "../TestBase.h"
#include "../../src/Models/User.h"
#include "../../src/Models/Group.h"

using namespace Models;

TEST_CASE("Constructor") {
    SECTION("has unique id") {
        auto* group = new Group;
        REQUIRE(!empty(group->getId()));
    }
}

TEST_CASE("Adding User") {
    SECTION("Single User") {
        auto* group = new Group;
        auto* user = new User(group);

        group->addUser(user);

        REQUIRE(group->getUsers()[0] == user);
    }

    SECTION("Multiple Users") {
        auto* group = new Group;
        auto* user1 = new User(group);
        auto* user2 = new User(group);

        group->addUser(user1);
        group->addUser(user2);

        REQUIRE(group->getUsers()[0] == user1);
        REQUIRE(group->getUsers()[1] == user2);
    }
}

TEST_CASE("Getting Users") {
    SECTION("Get by id found") {
        auto* group = new Group;
        auto* user = new User(group);

        group->addUser(user);

        REQUIRE(group->getUser(user->getId()) == user);
    }

    SECTION("Get by id not found") {
        auto* group = new Group;

        REQUIRE(!group->getUser("random-id"));
    }

    SECTION("Has user true") {
        auto* group = new Group;
        auto* user = new User(group);

        group->addUser(user);

        REQUIRE(group->hasUser(user->getId()));
    }

    SECTION("Has user false") {
        auto* group = new Group;

        REQUIRE(!group->hasUser("random-id"));
    }
}

TEST_CASE("Removing Users") {
    SECTION("Remove user also destructor the user") {
        auto* group = new Group;
        auto* user = new User(group);

        group->addUser(user);

        REQUIRE(group->getUsers()[0] == user);

        group->removeUser(user->getId());

        REQUIRE(empty(group->getUsers()));
    }

    SECTION("Remove user that doesn't exist is ignored") {
        auto* group = new Group;
        group->removeUser("random-id");
    }
}
