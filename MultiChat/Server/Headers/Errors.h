//
//  Errors.h
//  Chat
//
//  Created by vlad on 24.11.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#ifndef Errors_h
#define Errors_h

#include <stdio.h>
void socketError(void);
void bindError(void);
void listenError(void);
void selectError(void);
void newClientWasNotAccepted(void);
void connectionError(void);
#endif /* Errors_h */
