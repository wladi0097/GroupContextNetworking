//
// Created by wlad on 15.12.19.
//

#ifndef NET_SOCKET_H
#define NET_SOCKET_H


#include "User.h"
#include "Group.h"

class Socket {
 public:
    void sendToUser();
    void sendToGroup();
    void removeUser();
    Models::Group getGroup();
    Models::User getUser();
};


#endif  // NET_SOCKET_H
