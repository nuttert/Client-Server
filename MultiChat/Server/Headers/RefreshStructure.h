//
//  RefreshStructure.h
//  Chat
//
//  Created by vlad on 29.11.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <pthread.h>
typedef pthread_t Thread;
@interface RefreshStructure : NSObject
+(void)startServerProcessing;
@end
