//
//  Connection.h
//  Chat
//
//  Created by vlad on 24.11.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#ifndef Connection_h
#define Connection_h
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
typedef struct timeval Timer;
typedef struct sockaddr_in Address;
typedef fd_set  SetOfSockets;

struct Text;
typedef struct Text* PointerToText;
typedef struct Text{
    char* text;
    long int textSize;
    int clientNumber;
    int ourSocketNumber;
    int receiverSocketNumber;
    
}Text;

typedef struct{
    int number;
    int socket;
} ClientSocketAccordingClientNumber;

typedef ClientSocketAccordingClientNumber* PointerToSocketNumberType;
typedef enum{
    False = 0,True = 1
}boolean;

typedef enum{
    portNumber = 8070,
    clientMaximum = 5,
    bufferSize = 2
} Constants;

typedef enum{
    thereIsConnection = 1,
    writeAndRead = 2
}clientConnection;

void clearSetOfSockets(SetOfSockets* set);
void removeSocketFromSetOfSockets(int socket,SetOfSockets* set);
void setSocketToSetOfSockets(int socket,SetOfSockets* set);
boolean isSetSocketInSetOfSockets(int socket,SetOfSockets* set);

void startServer(PointerToText text);
void messageToAnotherClients(int numberOfClinets,
                             SetOfSockets* allClients,
                             PointerToText text,
                             long int textSize,
                             int listener,
                             int sender,
                             PointerToSocketNumberType* clientSocketAccordingClientNumber);
void textConstructor(PointerToText text);
int creatSocket(void);
#endif /* Connection_h */
