//
//  ClientConnection.h
//  MultiChat
//
//  Created by vlad on 29.11.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#ifndef ClientConnection_h
#define ClientConnection_h
#include "Connection.h"
#include <stdio.h>
void commantFromClientModelToUI(PointerToText text);
void startClient(PointerToText text);
void  sendTextToServerFromModel(PointerToText text);
void textDestructor(PointerToText text);
typedef struct{
    int clientNumber;
    int socket;
}SocketDescriptor;
typedef struct{
    SocketDescriptor* sockets;
    int amountOFSocket;
}SocketDescriptors;
#endif /* ClientConnection_h */
