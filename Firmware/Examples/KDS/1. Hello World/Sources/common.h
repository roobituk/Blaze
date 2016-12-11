
#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Include the generic type definitions
#include "types.h"

#define true												1
#define false												0

// Macro to enable interrupts
#define SEI 												__asm("CPSIE i")

#define ASSERT(x)

#endif
