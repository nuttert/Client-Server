//
//  ClientConnection.c
//  MultiChat
//
//  Created by vlad on 29.11.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#include "ClientConnection.h"
#include <stdio.h>
#include "Connection.h"
#include "Errors.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/time.h>
#include <mm_malloc.h>
#import "CtoObjectiveClient.h"
void completedText(PointerToText text);

//2. Connect to server
void connectToServer(int socket,Address* address){
    memset(address,0,sizeof(*address));
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(portNumber);
    if(connect(socket,(struct sockaddr*) address, sizeof(*address))< 0) connectionError();
}

//________________________________________________________________________________________________________________
//3 . Message receiving
void receivingMessage(PointerToText text,
                      char buffer[bufferSize],
                      int ourSocket){
    long int oldTextSize=0;
    long int readingSize;
    text->textSize += bufferSize;
    text->text =  realloc(text->text, sizeof(char)*(text->textSize+1));
    oldTextSize = strlen(text->text);
    readingSize =  read(ourSocket,buffer ,bufferSize);
    if(readingSize == 0){
        shutdown(ourSocket,writeAndRead);
        close(ourSocket);
        return;
    }
    buffer[readingSize] = '\0';
    strcpy(&(text->text[oldTextSize]), buffer);
    text->text[oldTextSize+readingSize] = '\0';
    if(strchr(buffer, '\n')) completedText(text);
}

//4. Completed text
void completedText(PointerToText text){
    commantFromClientModelToUI(text);
    textConstructor(text);
}
//5. Save socket
SocketDescriptors sockets;
void appendNewSocket(int newSocket,int clientNumber){
    static int amountOfSockets = 0;
    if(amountOfSockets == 0){
      sockets.sockets = malloc(sizeof(SocketDescriptor));
      sockets.amountOFSocket = 1;
    }
    else sockets.sockets = realloc(sockets.sockets,sizeof(char)*(amountOfSockets+1));
    sockets.sockets[amountOfSockets].clientNumber = clientNumber;
    sockets.sockets[amountOfSockets].socket = newSocket;
    amountOfSockets++;
    sockets.amountOFSocket = amountOfSockets;
}
int getSocket(int clientNumber){
    if (sockets.amountOFSocket < clientNumber)socketError();
    return sockets.sockets[clientNumber-1].socket;
}

//6. write text
void sendTextToServerFromModel(PointerToText text){
    long int lenghtOfText = strlen(text->text);
    int ourSocket = getSocket(text->clientNumber);
    if(ourSocket == 0)socketError();
    if(write(ourSocket, text->text, lenghtOfText)<0)connectionError();
}
//7. text destructor
void textDestructor(PointerToText text){
    free(text->text);
    free(text);
}

//START CLIENT_____________________________________________________________________________________________________
void startClient(PointerToText text){
    if(text == NULL)socketError();
    int clientSocket = creatSocket();                                //1. socket creating
    text->ourSocketNumber = clientSocket;
    appendNewSocket(clientSocket, text->clientNumber);
    Address serverAddress;
    connectToServer(clientSocket,&serverAddress);                    //2. Connect
    char buffer[bufferSize+1];
    textConstructor(text);
    while (thereIsConnection) {
        receivingMessage(text, buffer, clientSocket);
    }
}



