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


#include <GroupManager.h>
#include <User.h>
#include "WebSocket.h"
#include "../../deps/uWebSockets/src/App.h"

void WebSocket::init() {
    std::unique_ptr<GroupManager> gm = std::make_unique<GroupManager>();

    struct UserData {
        Models::User *user;
    };

    auto open = [&gm](auto *ws, auto *req) {
        auto url = req->getUrl();

        if (url == "/new") {
            Models::User* newUser = gm->createGroup();
            static_cast<UserData *>(ws->getUserData())->user = newUser;
            return;
        }

        if (url.length() == 5) {
            Models::Group *group = gm->getGroup(std::string(req->getUrl()));
            if (group != nullptr) {
                static_cast<UserData *>(ws->getUserData())->user = gm->joinGroup(group);
                return;
            }
        }

        ws->end(5, "Unsupported Open request");
    };

    auto message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
        auto *userdata = static_cast<UserData *>(ws->getUserData());
        if (userdata->user == nullptr) {
            ws->end(2, "err: unknown user");
            return;
        }
    };

    uWS::App().ws<UserData>("/*", {
            .compression = uWS::SHARED_COMPRESSOR,
            .maxPayloadLength = 16 * 1024 * 1024,
            .idleTimeout = 100,
            .maxBackpressure = 1 * 1024 * 1204,
            .open = open,
            .message = message
    }).listen(9001, [](auto *token) {
        if (token) {
            std::cout << "Listening on port " << 9001 << std::endl;
        }
    }).run();
}