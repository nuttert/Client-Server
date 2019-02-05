//
//  main.c
//  Chat
//
//  Created by vlad on 24.11.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#include <stdio.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/time.h>
#include <mm_malloc.h>
#include "CtoObjective-C.h"
#include "Connection.h"
#include "Errors.h"

void appendNewPairSocketAndNumber(PointerToSocketNumberType* tuple,int newSocket,int newNumber);
int getClientNumer(PointerToSocketNumberType* clientSocketAccordingClientNumber,int socket);
void appendStringToStart(PointerToText text,char* string);
void appendStringToStartOfAnotherString(char** text,char* string);
void translarotIntToString(int number,char** string,long size);
//PREPARING_____________________________________________________________________________________________________
//1. socket creating
int creatSocket(){
    int sock;
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) socketError();
    return sock;
}
//2. Binding a socket to an address
void socketBinding(int socket,Address* address){
    memset(address,0,sizeof(*address));
    address->sin_family = AF_INET;
    address->sin_port = htons(portNumber);
    int option = 1;
    setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if(bind(socket,(struct sockaddr*) address, sizeof(*address)) < 0) bindError();
}
//3. Server is ready to accept requests
void serverIsGoingToListen(int mainSocket){
    if(listen(mainSocket, clientMaximum)<0) listenError();
}


//Function for set of sockets//////////
void clearSetOfSockets(SetOfSockets* set){FD_ZERO(set);}
void removeSocketFromSetOfSockets(int socket,SetOfSockets* set){FD_CLR(socket,set);}
void setSocketToSetOfSockets(int socket,SetOfSockets* set){FD_SET(socket,set);}

boolean isSetSocketInSetOfSockets(int socket,SetOfSockets* set){return FD_ISSET(socket,set);}
///////////////////////////////////////

//4. Select active clients
int weedOutPassiveClients(int numberOfClinets,SetOfSockets* clients){
    Timer timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    int amountOfActiveClients = select(numberOfClinets+1, clients, NULL, NULL,&timeout);
    if (amountOfActiveClients < 0) selectError();
    return amountOfActiveClients;
}


//5. Set a new client
void setNewClient(int listener,
                  SetOfSockets* allClients,
                  int* numberOfClinets,
                  PointerToSocketNumberType* clientSocketAccordingClientNumber){
    static int sizeOftuple = 1;
    Address newClientAddress;
    unsigned lenghtOfAddress = sizeof(newClientAddress);
    memset(&newClientAddress,0,lenghtOfAddress);
    int newClient = accept(listener, (struct sockaddr*) &newClientAddress,&lenghtOfAddress);
    if (newClient<0) newClientWasNotAccepted();
    setSocketToSetOfSockets(newClient, allClients);
    if (newClient > *numberOfClinets) *numberOfClinets = newClient;
    appendNewPairSocketAndNumber(clientSocketAccordingClientNumber, newClient, sizeOftuple);
    sizeOftuple++;
}

//5. Processing old clients
void proccessingOldClients(int numberOfClinets,
                           SetOfSockets* activeClients,
                           SetOfSockets* allClients,
                           char buffer[],
                           int listener,
                           PointerToText text,
                           PointerToSocketNumberType* clientSocketAccordingClientNumber){
    
    for(int currentClient = 0;currentClient < numberOfClinets+1;currentClient++){
        
        if(currentClient != listener &&
           isSetSocketInSetOfSockets(currentClient, activeClients)){
            
            SetOfSockets currentClientInSet;
            setSocketToSetOfSockets(currentClient, &currentClientInSet);
            long int oldTextSize=0;
            long int readingSize;
               text->textSize += bufferSize;
                text->text =  realloc(text->text, sizeof(char)*(text->textSize+1));
                oldTextSize = strlen(text->text);
                readingSize =  read(currentClient,buffer, bufferSize);
                if(readingSize == 0){
                    shutdown(currentClient,writeAndRead);
                    close(currentClient);
                    removeSocketFromSetOfSockets(currentClient, allClients);
                    return;
                }
                buffer[readingSize] = '\0';
                strcpy(&(text->text[oldTextSize]), buffer);
                text->text[oldTextSize+readingSize] = '\0';
            if(strchr(buffer, '\n'))
            messageToAnotherClients(numberOfClinets,
                                    allClients,
                                    text,
                                    readingSize,
                                    listener,
                                    currentClient,
                                    clientSocketAccordingClientNumber);
        }
    }
}

//6. A client sends a message to another clients
void messageToAnotherClients(int numberOfClinets,
                             SetOfSockets* allClients,
                             PointerToText text,
                             long int textSize,
                             int listener,
                             int sender,
                             PointerToSocketNumberType* clientSocketAccordingClientNumber){
    int senderNumber = getClientNumer(clientSocketAccordingClientNumber, sender);
    
    char* userString = NULL;
    appendStringToStartOfAnotherString(&userString, "User ");
    translarotIntToString(senderNumber,&userString,strlen(userString));
    appendStringToStartOfAnotherString(&userString, ": ");
    
    appendStringToStart(text,userString);
    
    
    for(int currentClient = 0;currentClient < numberOfClinets+1;currentClient++){
        if(currentClient != listener &&
           isSetSocketInSetOfSockets(currentClient, allClients)&&
           currentClient!=sender){
            write(currentClient,text->text, text->textSize);
            printf("%s",text->text);
            commantFromServerModelToUI(text);
        }
    }
    textConstructor(text);
}
//7. Text initializer________________________________________________________________________________________________________________
void textConstructor(PointerToText text){
    free(text->text);
    text->textSize = 0;
    text->text = malloc(sizeof(char));
    text->text[0] = 0;
}

//8. Append new string to start
void appendStringToStart(PointerToText text,char* string){
    unsigned long stringLenght = strlen(string);
    char* tempText = malloc(stringLenght+text->textSize+1);
    strcpy(tempText, string);
    strcpy(tempText+stringLenght, text->text);
    free(text->text);
    text->textSize += stringLenght;
    tempText[text->textSize] = '\0';
    text->text = tempText;
    free(string);
}
void appendStringToStartOfAnotherString(char** text,char* string){
    unsigned long stringLenght = strlen(string);
    unsigned long textSize = 0;
    if(*text != NULL) textSize = strlen(*text);
    char* tempText = malloc(stringLenght+textSize+1);
    if(*text != NULL)
        strcpy(tempText, *text);
    strcpy(tempText+textSize, string);
    free(*text);
    textSize += stringLenght;
    tempText[textSize] = '\0';
    *text = tempText;
}
//9. Translator int to string
void translarotIntToString(int number,char** string,long size){
   int lenght = snprintf( NULL, 0, "%d", number);
    *string = realloc(*string, sizeof(char)*(lenght+size));
    char* tempString = malloc(lenght+1);
    sprintf(tempString, "%d",number);
    if (lenght!=0)  {
        strcpy(*string+size, tempString);}
    free(tempString);
}
//10. Append new symbol
void appendNewPairSocketAndNumber(PointerToSocketNumberType* tuple,int newSocket,int newNumber){
    static int sizeOftuple = 0;
    if (sizeOftuple == 0)*tuple = malloc(sizeof(ClientSocketAccordingClientNumber)*2);
    else *tuple = realloc(*tuple,sizeof(ClientSocketAccordingClientNumber)*(sizeOftuple+2));
    (*tuple)[sizeOftuple].number = newNumber;
    (*tuple)[sizeOftuple].socket = newSocket;
    (*tuple)[sizeOftuple+1].socket = 0;
    (*tuple)[sizeOftuple+1].number = 0;
    sizeOftuple++;
    PointerToSocketNumberType carriage = *tuple;
    while(carriage->socket!=0){
    printf("Number: %d Socket: %d\n",carriage->number,carriage->socket);
         carriage++;
    }
}
//11.
int getClientNumer(PointerToSocketNumberType* clientSocketAccordingClientNumber,int socket){
    PointerToSocketNumberType carriage = *clientSocketAccordingClientNumber;
   while((carriage)->socket!=0){
        printf("Number: %d\n Socket: %d",carriage->number,carriage->socket);
        if(carriage->socket == socket){
            return carriage->number;
        }
        carriage++;
    }
    return 0;
}



//________________________________________________________________________________________________________________



//________________________________________________________________________________________________________________

//START SERVER_____________________________________________________________________________________________________
void startServer(PointerToText text){
    if(text == NULL)socketError();
    int listener = creatSocket();                                    //1. socket creating
    Address serverAddress;
    socketBinding(listener, &serverAddress);                         //2. Binding a socket to an address
    serverIsGoingToListen(listener);
    //3. Server is ready to accept requests
    SetOfSockets allClients,activeClients;                           //4. Declaration of client sets
    
    clearSetOfSockets(&allClients);                                  //5. Default initialization of client sets
    clearSetOfSockets(&activeClients);
    
    setSocketToSetOfSockets(listener,&allClients);                   //6. Set listener into set of sockets
    PointerToSocketNumberType clientSocketAccordingClientNumber = NULL;
    int maxNumberAmongSockets = listener;                            //7. Let be max descriptor is listener at first
    char buffer[bufferSize+1];
    textConstructor(text);
    while (thereIsConnection) {
        activeClients = allClients;                                  //8. Let be all clients are active
        weedOutPassiveClients(maxNumberAmongSockets,&activeClients); //9. Now active clients are really active
        
        if(isSetSocketInSetOfSockets(listener, &activeClients))
            setNewClient(listener,
                         &allClients,
                         &maxNumberAmongSockets,
                         &clientSocketAccordingClientNumber);          //10. Got a new client
        
        proccessingOldClients(maxNumberAmongSockets,
                              &activeClients,
                              &allClients,
                              buffer,
                              listener,
                              text,
                              &clientSocketAccordingClientNumber);      //11. Processing old clients
    }
    
}
//________________________________________________________________________________________________________________
