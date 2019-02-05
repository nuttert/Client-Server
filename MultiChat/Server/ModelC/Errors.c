//
//  Errors.c
//  Chat
//
//  Created by vlad on 24.11.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#include "Errors.h"
#include <stdlib.h>
void socketError(){
    perror("socket error");
    exit(1);
}
void bindError(){
    perror("bind error");
    exit(1);
}
void listenError(){
    perror("listen error");
    exit(1);
}
void selectError(){
    perror("select error");
    exit(1);
}
void newClientWasNotAccepted(){
    perror("accept error");
    exit(1);
};
void connectionError(){
    perror("connection error");
    exit(1);
}
