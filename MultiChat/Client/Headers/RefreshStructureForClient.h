//
//  RefreshStructureForClient.h
//  MultiChat
//
//  Created by vlad on 29.11.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <pthread.h>
typedef pthread_t Thread;
void commandFromUIToClientModel(NSString* text,int clientNumber);
void startClientProcessing(int numberOfClient);
@interface RefreshStructureForClient : NSObject
@end
