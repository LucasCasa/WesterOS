#ifndef MODULELOADER_H
#define MODULELOADER_H

#include "sys_lib.h"

void loadModules(void * payloadStart, void ** moduleTargetAddress);

#endif