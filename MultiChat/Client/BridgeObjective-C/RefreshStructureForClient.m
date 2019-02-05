//
//  RefreshStructure.m
//  Chat
//
//  Created by vlad on 29.11.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#import "RefreshStructureForClient.h"
#import <pthread.h>
#import "MultiChat-Swift.h"
#import "CtoObjective-C.h"
#import "ClientConnection.h"
#import "CtoObjectiveClient.h"
void createThreadForSending(void* data);


void commantFromClientModelToUI(PointerToText text){
    int clientNumber = text->clientNumber;
    if (clientNumber == 0) return;
    ClientViewController* controller = ClientViewController.referenceToSelf[clientNumber-1];
    NSString* newText = [NSString stringWithUTF8String:text->text];
    controller.labelText = newText;

}
void commandFromUIToClientModel(NSString* text,int clientNumber){
    PointerToText textForModel = malloc(sizeof(Text));
    textForModel->clientNumber = clientNumber;
    textForModel->textSize = text.length;
    if(textForModel->textSize < 1) return;
    textForModel->text = malloc(sizeof(char)*(textForModel->textSize+2));
    strcpy(textForModel->text, text.UTF8String);
    textForModel->text[textForModel->textSize] = '\n';
    textForModel->text[textForModel->textSize+1] = '\0';
    createThreadForSending(textForModel);
}
//FUNCTION FOR A THREAD
void* startClientInOtherThread(void* data){
    startClient(data);
    return 0;
}

void* sendTextToServer(void* data){
    sendTextToServerFromModel(data);
    textDestructor(data);
    return 0;
}

//CREATEING NEW THREAD
void createThreadForClient(void* data,Thread* thread){
    pthread_create(thread, NULL, startClientInOtherThread, data);
}

void createThreadForSending(void* data){
    Thread thread;
    pthread_create(&thread, NULL, sendTextToServer, data);
}


@implementation RefreshStructureForClient

void startClientProcessing(int numberOfClient){
    if(ClientViewController.referenceToSelf == NULL) perror("Main view controller was never initialized");
    PointerToText text = malloc(sizeof(Text));
    text->text = NULL;
    text->clientNumber = 0;
    textConstructor(text);
    text->clientNumber = numberOfClient;
    Thread clientThread;
    createThreadForClient(text,&clientThread);
}
@end
