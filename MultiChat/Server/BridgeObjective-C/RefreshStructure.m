//
//  RefreshStructure.m
//  Chat
//
//  Created by vlad on 29.11.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#import "RefreshStructure.h"
#import <pthread.h>
#import "Connection.h"
#import "MultiChat-Swift.h"
#import "CtoObjective-C.h"

void commantFromServerModelToUI(PointerToText text){
    MainViewController* controller = MainViewController.referenceToSelf;
    controller.labelText = [NSString stringWithUTF8String:text->text];
}

void* startServerInOtherThread(void* data){
    startServer(data);
    return 0;
}

void createThread(void* data,Thread* thread){
    pthread_create(thread, NULL, startServerInOtherThread, data);
}

@implementation RefreshStructure

+(void)startServerProcessing{
    static bool serverWasStarted = false;
    if(serverWasStarted)return;
    if(MainViewController.referenceToSelf == NULL) perror("Main view controller was never initialized");
    PointerToText text = malloc(sizeof(Text));
    text->text = NULL;
    textConstructor(text);
    Thread serverThread;
    createThread(text,&serverThread);
}
@end
