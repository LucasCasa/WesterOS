#ifndef _SLEEP_H
#define _SLEEP_H

#define MAX_SLEEP    10

#include "process.h"
#include "handlers.h"
#include <stdint.h>

typedef struct slp{
   Process* p;
   int msec;
}slp;




#endif
