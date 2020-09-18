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



#include "WebSocket.h"
#include "../../deps/uWebSockets/src/App.h"
#include <GroupManager.h>
#include <User.h>
#include "UserData.h"


void WebSocket::init() {
    std::unique_ptr<GroupManager> gm = std::make_unique<GroupManager>();


    auto setInitialValues = [](auto *res,  auto *req, auto *context, auto *user, auto *group) {
        res->template upgrade<UserData>(
                {
                    .user = user,
                    .group = group
                    },
                req->getHeader("sec-websocket-key"),
                req->getHeader("sec-websocket-protocol"),
                req->getHeader("sec-websocket-extensions"),
                context
        );
    };

    auto getUserData = [](auto *ws) -> UserData* {
        auto *userData = static_cast<UserData *>(ws->getUserData());
        if (userData->user == nullptr) {
            ws->end(2, "err: unknown user");
            return nullptr;
        }
        return userData;
    };

    auto upgrade = [&gm, &setInitialValues](auto *res, auto *req, auto *context) {
        auto url = req->getUrl();

        if (url == "/new") {
            Models::Group *group = gm->joinNewGroup();
            setInitialValues(res, req, context, group->getHost(), group);
            return;
        }

        if (url.length() == 5) {
            Models::Group *group = gm->getGroup(std::string(url).erase(0, 1));
            if (group != nullptr) {
                setInitialValues(res, req, context, gm->joinGroup(group, false), group);
                return;
            }
        }
    };

    auto open = [&getUserData](uWS::WebSocket<false, true> *ws) {
        UserData* userData = getUserData(ws);
        if (!userData) {return;}

        userData->user->setWebSocket(ws);
        ws->send(userData->user->getId());
    };

    auto message = [&gm, &getUserData](auto *ws, std::string_view message, uWS::OpCode opCode) {
        UserData* userData = getUserData(ws);
        if (!userData) {return;}

        gm->handleMessage(userData, message);
    };

    auto close = [&gm, getUserData](auto *ws, int code, std::string_view message) {
        UserData* userData = getUserData(ws);
        if (!userData) {return;}

        gm->handleLeave(userData);
    };

    uWS::App().ws<UserData>("/*", {
            .compression = uWS::SHARED_COMPRESSOR,
            .maxPayloadLength = 16 * 1024 * 1024,
            .idleTimeout = 1000,
            .maxBackpressure = 1 * 1024 * 1204,
            .upgrade = upgrade,
            .open = open,
            .message = message,
            .close = close
    }).listen(9001, [](auto *token) {
        if (token) {
            std::cout << "Listening on port " << 9001 << std::endl;
        }
    }).run();
}